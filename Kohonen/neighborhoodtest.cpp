#include <iostream>

#include "neighborhood.h"
#include "euclidiandistance.h"
#include "kohonenneuron.h"
#include "gaussianneighborhood.h"

using namespace std;


int main()
{
	Neighborhood<KohonenNeuron, GaussianNeighborhoodFunction, EuclidianDistance, double> littleNeighbour;
}