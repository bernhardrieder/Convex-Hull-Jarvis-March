#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <vector>

class ConvexHullComparison
{
public:
	ConvexHullComparison();
	~ConvexHullComparison();
	int Execute(int argc, char** argv);

private:
	bool parseCommandLine(int argc, char** argv);
	static void showUsage(const std::string& fileName);
	static std::vector<sf::Vector2f> loadInputData(const std::string& fileName);
	static std::vector<sf::Vector2f> createRandomData(int amount, float randomNumberLow, float randomNumberHigh);

	std::string m_inputPath;
	bool m_useGraphics;
	bool m_isModeSet;
};

