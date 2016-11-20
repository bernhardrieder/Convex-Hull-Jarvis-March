#include "JarvisMarch.h"



JarvisMarch::JarvisMarch()
{
}


JarvisMarch::~JarvisMarch()
{
}

std::vector<sf::Vector2f> JarvisMarch::CalculateConvexHull(const std::vector<sf::Vector2f>& dataArray)
{
	//Fake algorithmen, just for GUI testing purposes

	std::vector<sf::Vector2f> pointsToCheck(dataArray);
	std::vector<sf::Vector2f> hull;

	hull.push_back(dataArray[pointsToCheck.size()-1]);
	pointsToCheck.pop_back();

	if(OnHullPointFoundEvent != nullptr)
		OnHullPointFoundEvent(hull);

	int idx = 0;

	while(!pointsToCheck.empty())
	{
		if (idx % 3 == 0)
		{
			if (OnHullCandidateFoundEvent != nullptr)
			OnHullCandidateFoundEvent(pointsToCheck[idx]);
		}
		else
		{
			if (OnPointCheckEvent != nullptr)
				OnPointCheckEvent(pointsToCheck[idx]);
		}

		++idx;
		if (idx == pointsToCheck.size())
		{
			idx = 0;
			hull.push_back(dataArray[pointsToCheck.size() - 1]);
			if (OnHullPointFoundEvent != nullptr)
				OnHullPointFoundEvent(hull);
			pointsToCheck.pop_back();
		}
	}
	return hull;
}
