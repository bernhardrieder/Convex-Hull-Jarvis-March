#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <chrono>

class Visualization
{
public:
	Visualization() = delete;
	Visualization(const sf::Vector2f& preferedWindowSize, const sf::Vector2f& minCoord, const sf::Vector2f& maxCoord, const std::vector<sf::Vector2f>& vectorData, const float& pointSize, const long long& millisecondPause);
	~Visualization();

	void RenderPartialHull(const std::vector<sf::Vector2f>& hullPoints);
	void RenderCompleteHull(const std::vector<sf::Vector2f>& hullPoints);
	void RenderCheckLine(const sf::Vector2f& checkPoint);
	void RenderHullCandidateLine(const sf::Vector2f& candidatePoint);

	bool ShouldClose();

private:
	sf::Vector2i calculateWindowSize(const sf::Vector2f& preferedWindowSize, const sf::Vector2f& minCoord, const sf::Vector2f& maxCoord, const float& borderPercent = 0.1f);

	void draw() const;
	void handleEvents() const;

	float m_pointSize;
	long long m_millisecondPause = 20;
	float m_zoomFactor = 1.f;
	sf::Vector2f m_origin;

	sf::RenderWindow* m_renderWindow = nullptr;
	std::vector<sf::Vector2f> m_vectors;
	sf::CircleShape* m_currentAnchor = nullptr;
	sf::VertexArray* m_checkline = nullptr;
	sf::VertexArray* m_candidateLine = nullptr;
	sf::VertexArray* m_hull = nullptr;
	std::vector<sf::Drawable*> m_drawableVectors;
};

