#include "ConvexHullComparison.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "JarvisMarch.h"
#include <fstream>
#include <sstream>
#include "Visualization.h"
#include <iomanip>
#include <fstream>

inline float randomNumber(float low, float high)
{
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

ConvexHullComparison::ConvexHullComparison(): m_visualization(nullptr), m_useGraphics(false), m_isModeSet(false)
{
	srand(static_cast<unsigned int>(time(NULL)));
}

ConvexHullComparison::~ConvexHullComparison()
{
	if (m_visualization != nullptr)
		delete m_visualization;
}

int ConvexHullComparison::Execute(int argc, char** argv)
{
	if (!parseCommandLine(argc, argv)) return 0;

	JarvisMarch jarvisMarch;

	std::vector<sf::Vector2f> points = m_inputPath.size() > 0 ? loadInputData(m_inputPath) : createRandomData(100, -100, 100);

	if(m_useGraphics)
	{
		sf::Vector2f min(FLT_MAX, FLT_MAX);
		sf::Vector2f max(0, 0);

		for (const auto& p : points)
		{
			if (p.x < min.x)
				min.x = p.x;
			if (p.x > max.x)
				max.x = p.x;
			if (p.y < min.y)
				min.y = p.y;
			if (p.y > max.y)
				max.y = p.y;
		}

		m_visualization = new Visualization(sf::Vector2f(1024, 1024), min, max, points, 4, 30);
		jarvisMarch.OnHullPointFoundEvent = [&](const std::vector<sf::Vector2f>& hullpoints) { m_visualization->RenderPartialHull(hullpoints); };
		jarvisMarch.OnHullCompleteEvent = [&](const std::vector<sf::Vector2f>& hullpoints) { m_visualization->RenderCompleteHull(hullpoints); };
		jarvisMarch.OnPointCheckEvent = [&](const sf::Vector2f& checkPoint) { m_visualization->RenderCheckLine(checkPoint); };
		jarvisMarch.OnHullCandidateFoundEvent = [&](const sf::Vector2f& hullCandidatePoint) { m_visualization->RenderHullCandidateLine(hullCandidatePoint); };
	}

	auto startTime = std::chrono::high_resolution_clock::now();
	auto convexHull = jarvisMarch.GetConvexHull(points);
	auto endTime = std::chrono::high_resolution_clock::now() - startTime;

	if(!m_useGraphics)
	{
		std::cout << "Jarvis March took: ";
		printDuration(std::cout, endTime);
		std::cout << " (min:sec:millisec:nanosec)" << std::endl;
		printConvexHull(convexHull);
	} 
	else
	{
		while (!m_visualization->ShouldClose());
	}
	return 0;
}

int ConvexHullComparison::ExecuteTestingProtocol() const
{
	//just used for testing protocol
	JarvisMarch jarvisMarch;
	std::ofstream outputFile("testing_protocol.csv");
	if (outputFile.is_open())
	{
		int maxPoints = 1000000;
		std::vector<sf::Vector2f> randomPoints = createRandomData(maxPoints, -1000000000, 1000000000);

		outputFile << "datasize" << ';' << "JarvisMarch" << ';' << '\n';

		for (int testNumbers = 10; testNumbers <= maxPoints; testNumbers *= 10)
		{
			std::vector<sf::Vector2f> points(randomPoints.begin(), randomPoints.begin() + testNumbers);
			outputFile << testNumbers << ';';	
			auto startTime = std::chrono::high_resolution_clock::now();
			jarvisMarch.GetConvexHull(points);
			printDuration(outputFile, std::chrono::high_resolution_clock::now() - startTime);
			outputFile << '\n';
		}
	}

	std::cout << "Finished all tests!\n";
	getchar();
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

void ConvexHullComparison::printConvexHull(const std::vector<sf::Vector2f>& convexHull)
{
	std::cout << "Convex Hull Points:" << std::endl;
	for (auto hullPoint : convexHull)
	{
		std::cout << "[" << std::to_string(hullPoint.x) << "," << std::to_string(hullPoint.y) << "]" << std::endl;
	}
}

void ConvexHullComparison::printDuration(std::ostream& output, std::chrono::duration<double> diff) const
{
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(diff);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
	auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(diff);


	output << std::setfill('0') << std::setw(2) << minutes.count() << ":" << std::setfill('0') << std::setw(2) << seconds.count() << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << ":" << nanoseconds.count() << ";";
}
