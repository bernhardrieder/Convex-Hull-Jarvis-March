#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "Visualization.h"
#include <chrono>

class ConvexHullComparison
{
public:
	ConvexHullComparison();
	~ConvexHullComparison();
	int Execute(int argc, char** argv);
	int ExecuteTestingProtocol_RandomPoints() const;
	int ExecuteTestingProtocol_ExtremaFiles() const;

private:
	bool parseCommandLine(int argc, char** argv);
	static void showUsage(const std::string& fileName);
	static std::vector<sf::Vector2f> loadInputData(const std::string& fileName);
	static std::vector<sf::Vector2f> createRandomData(int amount, float randomNumberLow, float randomNumberHigh);
	static void printConvexHull(const std::vector<sf::Vector2f>& convexHull);
	void printDuration(std::ostream& output, std::chrono::duration<double> diff) const;

	Visualization* m_visualization;
	std::string m_inputPath;
	bool m_useGraphics;
	bool m_isModeSet;
};

