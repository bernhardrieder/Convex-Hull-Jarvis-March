#include "Visualization.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>


Visualization::Visualization(sf::VideoMode mode, const std::vector<sf::Vector2f>& vectorData) : m_vectors(vectorData)
{
	m_renderWindow = new sf::RenderWindow(mode, "Convex Hull Comparison");
	for (auto& a : m_vectors)
	{
		sf::CircleShape* shape = new sf::CircleShape(100.f);
		shape->setFillColor(sf::Color::Green);
		// set position etc ....
		m_drawableVectors.push_back(shape);
	}
}


Visualization::~Visualization()
{
	for (int i = 0; i < m_drawableVectors.size(); ++i)
	{
		delete m_drawableVectors[i];
	}
	if(m_renderWindow != nullptr)
		delete m_renderWindow;
}

void Visualization::RenderVectors() const
{
	draw(m_drawableVectors);
}

void Visualization::RenderNewPointOfCalculation(const sf::Vector2f& point)
{
	sf::CircleShape shape(10.f);
	shape.setFillColor(sf::Color::Blue);
	std::vector<sf::Drawable*> draws(m_drawableVectors.begin(), m_drawableVectors.end());
	draws.push_back(&shape);
	draw(draws);
}

void Visualization::draw(const std::vector<sf::Drawable*>& drawables) const
{
	if (m_renderWindow->isOpen())
	{
		handleEvents();

		m_renderWindow->clear();
		for(int i = 0; i < drawables.size(); ++i)
			m_renderWindow->draw(*drawables[i]);
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
