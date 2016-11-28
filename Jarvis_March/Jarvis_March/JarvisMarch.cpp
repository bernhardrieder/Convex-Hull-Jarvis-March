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

	auto tmpPoints = points;
	//init result
	std::vector<sf::Vector2f> convexHull;

	//get first hull point and startpoint - use leftmost point
	sf::Vector2f hullPoint = findLeftmost(tmpPoints);
	sf::Vector2f endPoint;
	int convexHullPointIndex;
	do
	{
		convexHullPointIndex = 0;
		//add current found hull point
		convexHull.push_back(hullPoint);
		if (OnHullPointFoundEvent != nullptr)
			OnHullPointFoundEvent(convexHull);

		//initial endpoint for a candidate edge on the hull
		endPoint = tmpPoints[0];
		for(int i = 1; i < tmpPoints.size(); ++i)
		{
			//iterate and check every point
			if (OnPointCheckEvent != nullptr)
				OnPointCheckEvent(tmpPoints[i]);

			if (endPoint == hullPoint || isOnTheLeftSideOfLine(hullPoint, tmpPoints[i], endPoint))
			{
				//if current point is on the left side of the vector from current hullpoint to endpoint then use this as next hull point candidate
				endPoint = tmpPoints[i];
				convexHullPointIndex = i;
				if (OnHullCandidateFoundEvent != nullptr)
					OnHullCandidateFoundEvent(endPoint);
			}
		}
		//assign new leftmost point as new hull point
		hullPoint = endPoint;
		//erase found convex hull point from list
		tmpPoints.erase(tmpPoints.begin() + convexHullPointIndex);
	} 
	//do until we wrapped around to the first hull point
	while (endPoint != convexHull[0]);

	if (OnHullCompleteEvent != nullptr)
		OnHullCompleteEvent(convexHull);

	return convexHull;
}

sf::Vector2f JarvisMarch::findLeftmost(const std::vector<sf::Vector2f>& dataArray)
{
	int leftmostIndex = 0;
	for(int i = 1; i < dataArray.size(); ++i)
	{
		//check for the smallest x and use this index for leftmost point
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

	//return cross product (if result < 0 its on the left side of the vector (to-from) and if result > 0 its on the right side)
	return x1*y2 - y1*x2 < 0; 
}
