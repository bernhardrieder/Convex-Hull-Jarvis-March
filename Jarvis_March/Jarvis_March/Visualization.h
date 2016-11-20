#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Visualization
{
public:
	Visualization() = delete;
	Visualization(sf::VideoMode mode, const std::vector<sf::Vector2f>& vectorData, float pointSize);
	~Visualization();

	void RenderHull(const std::vector<sf::Vector2f>& hullPoints);
	void RenderCheckLine(const sf::Vector2f& checkPoint);
	void RenderHullCandidateLine(const sf::Vector2f& candidatePoint);

private:
	void draw() const;
	void handleEvents() const;

	float m_pointSize;

	sf::RenderWindow* m_renderWindow = nullptr;
	std::vector<sf::Vector2f> m_vectors;
	sf::CircleShape* m_currentAnchor = nullptr;
	sf::VertexArray* m_checkline = nullptr;
	sf::VertexArray* m_candidateLine = nullptr;
	sf::VertexArray* m_hull = nullptr;
	std::vector<sf::Drawable*> m_drawableVectors;
};

