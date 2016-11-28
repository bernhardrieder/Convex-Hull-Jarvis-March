#include "ConvexHullComparison.h"

int main(int argc, char** argv) 
{
	ConvexHullComparison convexHull;
	return convexHull.Execute(argc, argv);
	//return convexHull.ExecuteTestingProtocol_RandomPoints();
	//return convexHull.ExecuteTestingProtocol_ExtremaFiles();
}
