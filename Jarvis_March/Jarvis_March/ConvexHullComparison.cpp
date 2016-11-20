#include "ConvexHullComparison.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "JarvisMarch.h"
#include <fstream>
#include <sstream>
#include "Visualization.h"

inline float randomNumber(float low, float high)
{
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

ConvexHullComparison::ConvexHullComparison(): m_useGraphics(false), m_isModeSet(false)
{
	srand(time(NULL));
}


ConvexHullComparison::~ConvexHullComparison()
{
}

int ConvexHullComparison::Execute(int argc, char** argv)
{
	if (!parseCommandLine(argc, argv)) return 0;

	std::vector<sf::Vector2f> vectorsData = m_inputPath.size() > 0 ? loadInputData(m_inputPath) : createRandomData(10, 50, 462);
	if (vectorsData.size() == 0) return 0;

	//get distance between furthest points to determin the 
	Visualization visualization(sf::VideoMode(512, 512), vectorsData);
	JarvisMarch jarvisMarch;
	if(m_useGraphics)
	{
		//change window on each On.. event
		jarvisMarch.OnChangePointOfCalculation = [&](const sf::Vector2f& point) { visualization.RenderNewPointOfCalculation(point); };
		visualization.RenderVectors();
	}
	jarvisMarch.CalculateConvexHull(vectorsData);

	return 0;
}

bool ConvexHullComparison::parseCommandLine(int argc, char** argv)
{
	if (argc < 3)
	{
		showUsage(argv[0]);
		return false;
	}
	bool error = false;
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (arg == "--mode" && i + 1 < argc)
		{
			std::string mode = argv[++i];
			if (mode == "performance")
				m_useGraphics = false;
			else if (mode == "visual")
				m_useGraphics = true;
			else
				error = true;
			m_isModeSet = true;
		}
		else if	(arg == "--load" && i + 1 < argc)
			m_inputPath = argv[++i];
		else
			error = true;
	}
	if(error || !m_isModeSet)
	{
		std::cout << "[ERROR]: Input error!\n";
		return false;
	}
	return true;
}

void ConvexHullComparison::showUsage(const std::string& fileName)
{
	std::cerr << "Usage: " << fileName << " <option(s)> parameters\n"
		<< "Options:\n"
		<< "\t--mode\t\t\tMode of computation: --mode performance (outputs convex hull + time measurements on console)"
		<< "\t\t\t\t --mode visual"
		<< "\t--load NAME\t\tOPTIONAL: Load an input: --load NAME (where NAME is a filename with the extension '.gol')\n"
		<< std::endl;
	getchar();
}

std::vector<sf::Vector2f> ConvexHullComparison::loadInputData(const std::string& filePath)
{
	int amount = 0; 
	std::vector<sf::Vector2f> data;

	std::string line;
	std::ifstream inputFile(filePath);
	if (inputFile.is_open())
	{
		//get amount
		if (getline(inputFile, line))
			amount = std::stoi(line);

		data.resize(amount);

		//get vectors
		int vectorCount = 0;
		while(getline(inputFile, line))
		{
			std::stringstream ss(line);
			std::string point;
			int pointCount = 0;
			while (getline(ss, point, ','))
			{
				if (pointCount == 0)
					data[vectorCount].x = std::stof(point);
				else if (pointCount == 1)
					data[vectorCount].y = std::stof(point);

				pointCount++;
			}
			++vectorCount;
		}
		inputFile.close();
	}
	return std::move(data);
}

std::vector<sf::Vector2f> ConvexHullComparison::createRandomData(int amount, float randomNumberLow, float randomNumberHigh)
{
	std::vector<sf::Vector2f> randomData;
	randomData.resize(amount);
	for(int i = 0; i < amount; ++i)
	{
		randomData[i].x = randomNumber(randomNumberLow, randomNumberHigh);
		randomData[i].y = randomNumber(randomNumberLow, randomNumberHigh);
	}
	return std::move(randomData);
}
