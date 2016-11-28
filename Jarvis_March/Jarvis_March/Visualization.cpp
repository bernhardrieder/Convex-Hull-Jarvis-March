#include "Visualization.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <thread>

#define GREEN sf::Color(0,200,0,255);

Visualization::Visualization(const sf::Vector2f & preferedWindowSize, const sf::Vector2f & minCoord, 
	const sf::Vector2f & maxCoord, const std::vector<sf::Vector2f>& vectorData, const float & pointSize, 
	const long long& millisecondPause) : m_vectors(vectorData), m_pointSize(pointSize), m_millisecondPause(millisecondPause)
{
	//get the adjusted windowsize
	sf::Vector2i windowSize = calculateWindowSize(preferedWindowSize, minCoord, maxCoord, .2f);
	m_renderWindow = new sf::RenderWindow(sf::VideoMode(windowSize.x,windowSize.y), "Convex Hull Comparison");

	//add lines for the axis
	sf::VertexArray* horizontalAxis = new sf::VertexArray(sf::LinesStrip, 2);
	(*horizontalAxis)[0].position = sf::Vector2f(0, m_origin.y);
	(*horizontalAxis)[0].color = sf::Color(0, 0, 0, 100);
	(*horizontalAxis)[1].position = sf::Vector2f(static_cast<float>(windowSize.x), m_origin.y);
	(*horizontalAxis)[1].color = sf::Color(0, 0, 0, 100);

	sf::VertexArray* verticalAxis = new sf::VertexArray(*horizontalAxis);
	(*verticalAxis)[0].position = sf::Vector2f(m_origin.x, 0);
	(*verticalAxis)[1].position = sf::Vector2f(m_origin.x, static_cast<float>(windowSize.y));

	m_drawableVectors.push_back(horizontalAxis);
	m_drawableVectors.push_back(verticalAxis);

	for (auto& pos : m_vectors)
	{
		//create a new circle for every point
		sf::CircleShape* shape = new sf::CircleShape(m_pointSize);
		//configure circle
		shape->setOrigin(shape->getRadius(), shape->getRadius());
		shape->setPosition(pos*m_zoomFactor + m_origin);

		//setup circle visuals
		shape->setFillColor(sf::Color::Transparent);
		shape->setOutlineColor(sf::Color::Black);
		shape->setOutlineThickness(2.f);

		//store the circles in a list
		m_drawableVectors.push_back(shape);
	}

	//create and configure lines
	m_checkline = new sf::VertexArray(sf::LinesStrip, 2);
	(*m_checkline)[0].color = GREEN;
	(*m_checkline)[1].color = GREEN;

	m_candidateLine = new sf::VertexArray(sf::LinesStrip, 2);
	(*m_candidateLine)[0].color = sf::Color::Black;
	(*m_candidateLine)[1].color = sf::Color::Black;
}


Visualization::~Visualization()
{
	for (int i = 0; i < m_drawableVectors.size(); ++i)
	{
		delete m_drawableVectors[i];
	}
	delete m_renderWindow;
	delete m_candidateLine;
	delete m_checkline;
	delete m_currentAnchor;
	delete m_hull;
}

// Renders the hull with a red line from hullpoints[0], hullpoints[1] .. hullpoints[n]
// and renders the last point of the hull with a blue circle
void Visualization::RenderPartialHull(const std::vector<sf::Vector2f>& hullPoints)
{
	//override the existing hull
	m_hull = new sf::VertexArray(sf::LinesStrip, hullPoints.size());
	//save all positions
	for (unsigned int i = 0; i < hullPoints.size(); ++i)
	{
		(*m_hull)[i].position = hullPoints[i] * m_zoomFactor + m_origin;
		(*m_hull)[i].color = sf::Color::Red;
	}

	if (m_currentAnchor == nullptr)
	{
		//create new circle for the current anchor
		m_currentAnchor = new sf::CircleShape(m_pointSize);
		m_currentAnchor->setOrigin(m_currentAnchor->getRadius(), m_currentAnchor->getRadius());
		m_currentAnchor->setFillColor(sf::Color::Blue);
	}

	//set the current anchor to the position of the last point
	m_currentAnchor->setPosition(*(hullPoints.end() - 1)*m_zoomFactor + m_origin);

	//reset the check- and candidate line
	(*m_checkline)[0].position = m_currentAnchor->getPosition();
	(*m_checkline)[1].position = m_currentAnchor->getPosition();
	(*m_candidateLine)[0].position = m_currentAnchor->getPosition();
	(*m_candidateLine)[1].position = m_currentAnchor->getPosition();

	draw();
}

void Visualization::RenderCompleteHull(const std::vector<sf::Vector2f>& hullPoints)
{
	//override the current hull
	m_hull = new sf::VertexArray(sf::LinesStrip, hullPoints.size()+1);

	//save all positions
	for (unsigned int i = 0; i < hullPoints.size(); ++i)
	{
		(*m_hull)[i].position = hullPoints[i] * m_zoomFactor + m_origin;
		(*m_hull)[i].color = sf::Color::Red;
	}

	//set last point to be connected to the first one
	(*m_hull)[hullPoints.size()].position = hullPoints[0] * m_zoomFactor + m_origin;
	(*m_hull)[hullPoints.size()].color = sf::Color::Red;

	//delete shapes and explicitly set them to nullpointer
	delete m_currentAnchor;
	m_currentAnchor = nullptr;
	delete m_checkline;
	m_checkline = nullptr;
	delete m_candidateLine;
	m_candidateLine = nullptr;

	draw();
}

// Renders a green line between the current anchor and the checkPoint
void Visualization::RenderCheckLine(const sf::Vector2f & checkPoint)
{
	(*m_checkline)[0].position = m_currentAnchor->getPosition();
	(*m_checkline)[1].position = checkPoint * m_zoomFactor + m_origin;

	draw();
}

// Renders a black line between the current anchor and the candidatePoint
void Visualization::RenderHullCandidateLine(const sf::Vector2f & candidatePoint)
{
	(*m_checkline)[0].position = m_currentAnchor->getPosition();
	(*m_checkline)[1].position = candidatePoint * m_zoomFactor + m_origin;

	(*m_candidateLine)[0].position = m_currentAnchor->getPosition();
	(*m_candidateLine)[1].position = candidatePoint * m_zoomFactor + m_origin;

	draw();
}

bool Visualization::ShouldClose()
{
	handleEvents();
	return !m_renderWindow->isOpen();
}

// Calculates a proper windowsize to fit all the points into the prefered window size. 
// The window size will not exceed the prefered window size.
// The border percent determins the distance to the outer edge of the window
sf::Vector2i Visualization::calculateWindowSize(const sf::Vector2f & preferedWindowSize, 
	const sf::Vector2f & minCoord, const sf::Vector2f & maxCoord, const float& borderPercent)
{
	//create a vector2 that defines the whole area
	sf::Vector2f extends = maxCoord - minCoord;

	//calculate the aspectratio
	float aspectRatio = extends.x / extends.y;
	if (aspectRatio > 1)
		//take the wide side to calculate the necessary zoom
		m_zoomFactor = (preferedWindowSize.x) / extends.x;
	else
		//take the tall side...
		m_zoomFactor = (preferedWindowSize.y) / extends.y;

	//calculate the actual windowsize
	sf::Vector2i windowSize(static_cast<int>(extends.x*m_zoomFactor), static_cast<int>(extends.y*m_zoomFactor));

	//add a border
	float borderFactor = 1 + borderPercent;
	sf::Vector2f border = extends*m_zoomFactor / borderFactor - extends*m_zoomFactor;
	//zoom a away to make room for the border
	m_zoomFactor /= borderFactor;
	//create a vector that defines the origin
	m_origin = -minCoord * m_zoomFactor;
	//move the origin half of the border size away
	m_origin -= border*0.5f;

	return windowSize;
}

// Renders all the points and lines
void Visualization::draw() const
{
	if (m_renderWindow->isOpen())
	{
		handleEvents();

		m_renderWindow->clear(sf::Color::White);
		for(int i = 0; i < m_drawableVectors.size(); ++i)
			m_renderWindow->draw(*m_drawableVectors[i]);

		if (m_currentAnchor != nullptr)
			m_renderWindow->draw(*m_currentAnchor);

		if (m_hull != nullptr)
			m_renderWindow->draw(*m_hull);

		if (m_checkline != nullptr)
			m_renderWindow->draw(*m_checkline);

		if (m_candidateLine != nullptr)
			m_renderWindow->draw(*m_candidateLine);

		m_renderWindow->display();

		if(m_millisecondPause > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(m_millisecondPause));
	}
}

// Handles the SFML Events
void Visualization::handleEvents() const
{
	sf::Event event;
	while (m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_renderWindow->close();
	}
}