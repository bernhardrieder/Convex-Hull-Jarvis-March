#include "JarvisMarch.h"
#include <iostream>


JarvisMarch::JarvisMarch()
{
}


JarvisMarch::~JarvisMarch()
{
}

std::vector<sf::Vector2f> JarvisMarch::GetConvexHull(const std::vector<sf::Vector2f>& points) const
{
	if (points.size() < 3)
	{
		std::cerr << "ERROR: 3 points are required at least!" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<sf::Vector2f> convexHull;

	sf::Vector2f hullPoint = findLeftmost(points);
	sf::Vector2f endPoint;
	do
	{
		convexHull.push_back(hullPoint);
		if (OnHullPointFoundEvent != nullptr)
			OnHullPointFoundEvent(convexHull);

		endPoint = points[0];
		for(int i = 1; i < points.size(); ++i)
		{
			if (OnPointCheckEvent != nullptr)
				OnPointCheckEvent(points[i]);
			if (endPoint == hullPoint || isOnTheLeftSideOfLine(hullPoint, points[i], endPoint))
			{
				endPoint = points[i];
				if (OnHullCandidateFoundEvent != nullptr)
					OnHullCandidateFoundEvent(endPoint);
			}
		}
		hullPoint = endPoint;

	} 
	while (endPoint != convexHull[0]);

	return convexHull;
}

sf::Vector2f JarvisMarch::findLeftmost(const std::vector<sf::Vector2f>& dataArray)
{
	int leftmostIndex = 0;
	for(int i = 1; i < dataArray.size(); ++i)
	{
		if (dataArray[i].x < dataArray[leftmostIndex].x)
			leftmostIndex = i;
	}
	return dataArray[leftmostIndex];
}

bool JarvisMarch::isOnTheLeftSideOfLine(const sf::Vector2f& from, const sf::Vector2f& point, const sf::Vector2f& to)
{
	float x1 = to.x - from.x;
	float y1 = to.y - from.y;
	float x2 = point.x - from.x;
	float y2 = point.y - from.y;

	return x1*y2 - y1*x2 < 0; //cross product
}
