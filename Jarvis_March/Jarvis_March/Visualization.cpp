#include "Visualization.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>


Visualization::Visualization(sf::VideoMode mode, const std::vector<sf::Vector2f>& vectorData, const float pointSize) : m_vectors(vectorData), m_pointSize(pointSize)
{
	m_renderWindow = new sf::RenderWindow(mode, "Convex Hull Comparison");
	for (auto& a : m_vectors)
	{
		sf::CircleShape* shape = new sf::CircleShape(m_pointSize);
		shape->setOrigin(shape->getRadius(), shape->getRadius());
		shape->setPosition(a);

		shape->setFillColor(sf::Color::Transparent);
		shape->setOutlineColor(sf::Color::Black);
		shape->setOutlineThickness(2.f);
		// set position etc ....
		m_drawableVectors.push_back(shape);
	}
	m_checkline = new sf::VertexArray(sf::LinesStrip, 2);
	(*m_checkline)[0].color = sf::Color::Green;
	(*m_checkline)[1].color = sf::Color::Green;

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
	if(m_renderWindow != nullptr)
		delete m_renderWindow;
	delete m_candidateLine;
	delete m_checkline;
	delete m_currentAnchor;
}

void Visualization::RenderHull(const std::vector<sf::Vector2f>& hullPoints)
{
	m_hull = new sf::VertexArray(sf::LinesStrip, hullPoints.size());
	for (unsigned int i = 0; i < hullPoints.size(); ++i)
	{
		(*m_hull)[i].position = hullPoints[i];
		(*m_hull)[i].color = sf::Color::Red;
	}

	if (m_currentAnchor == nullptr)
	{
		m_currentAnchor = new sf::CircleShape(m_pointSize);
		m_currentAnchor->setOrigin(m_currentAnchor->getRadius(), m_currentAnchor->getRadius());
		m_currentAnchor->setFillColor(sf::Color::Blue);
	}
	m_currentAnchor->setPosition(*hullPoints.end());
}

void Visualization::RenderCheckLine(const sf::Vector2f & checkPoint)
{
	(*m_candidateLine)[0].position = m_currentAnchor->getPosition();
	(*m_candidateLine)[1].position = checkPoint;
}

void Visualization::RenderHullCandidateLine(const sf::Vector2f & candidatePoint)
{
	(*m_checkline)[0].position = m_currentAnchor->getPosition();
	(*m_checkline)[1].position = candidatePoint;
}

void Visualization::draw() const
{
	if (m_renderWindow->isOpen())
	{
		handleEvents();

		m_renderWindow->clear(sf::Color::White);
		for(int i = 0; i < m_drawableVectors.size(); ++i)
			m_renderWindow->draw(*m_drawableVectors[i]);
		if(m_currentAnchor != nullptr)
			m_renderWindow->draw(*m_currentAnchor);
		if (m_checkline != nullptr)
			m_renderWindow->draw(*m_checkline);
		if (m_candidateLine != nullptr)
			m_renderWindow->draw(*m_candidateLine);
		m_renderWindow->display();
	}
}

void Visualization::handleEvents() const
{
	sf::Event event;
	while (m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_renderWindow->close();
	}
}
