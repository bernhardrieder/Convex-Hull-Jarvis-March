#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <vector>

class JarvisMarch
{
public:
	JarvisMarch();
	~JarvisMarch();

	std::vector<sf::Vector2f> GetConvexHull(const std::vector<sf::Vector2f>& points) const;

	std::function<void(const sf::Vector2f&)> OnPointCheckEvent = nullptr;
	std::function<void(const sf::Vector2f&)> OnHullCandidateFoundEvent = nullptr;
	std::function<void(const std::vector<sf::Vector2f>&)> OnHullPointFoundEvent = nullptr;

private:
	static sf::Vector2f findLeftmost(const std::vector<sf::Vector2f>& DataArray);
	static bool isOnTheLeftSideOfLine(const sf::Vector2f& from, const sf::Vector2f& point, const sf::Vector2f& to);
};

