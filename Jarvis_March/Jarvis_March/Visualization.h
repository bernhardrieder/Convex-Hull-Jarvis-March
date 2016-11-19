#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Visualization
{
public:
	Visualization() = delete;
	Visualization(sf::VideoMode mode, const std::vector<sf::Vector2f>& vectorData);
	~Visualization();

	void RenderVectors() const;
	void RenderNewPointOfCalculation(const sf::Vector2f& point);

private:
	void draw(const std::vector<sf::Drawable*>& drawables) const;
	void handleEvents() const;

	sf::RenderWindow* m_renderWindow = nullptr;
	std::vector<sf::Vector2f> m_vectors;
	std::vector<sf::Drawable*> m_drawableVectors;
};

