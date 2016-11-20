#include "JarvisMarch.h"



JarvisMarch::JarvisMarch()
{
}


JarvisMarch::~JarvisMarch()
{
}

std::vector<sf::Vector2f> JarvisMarch::CalculateConvexHull(const std::vector<sf::Vector2f>& dataArray)
{
	if(OnChangePointOfCalculation != nullptr)
		OnChangePointOfCalculation(dataArray);

	while(true)
	{
		//do shit
	}
	return std::vector<sf::Vector2f>();
}
