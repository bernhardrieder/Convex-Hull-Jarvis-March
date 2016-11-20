#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <vector>

class JarvisMarch
{
public:
	JarvisMarch();
	~JarvisMarch();

	std::vector<sf::Vector2f> CalculateConvexHull(const std::vector<sf::Vector2f>& pDataArray);

	std::function<void(const sf::Vector2f&)> OnPointCheckEvent = nullptr;
	std::function<void(const sf::Vector2f&)> OnHullCandidateFoundEvent = nullptr;
	std::function<void(const std::vector<sf::Vector2f>&)> OnHullPointFoundEvent = nullptr;
};

