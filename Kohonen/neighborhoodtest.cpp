#include <iostream>

#include "neighborhood.h"
#include "kohonenneuron.h"
#include "gaussianneighborhood.h"


#include "../Common/euclidiandistance.h"

using namespace std;


int main()
{
	Neighborhood<KohonenNeuron, GaussianNeighborhoodFunction, EuclidianDistance, double> littleNeighbour;
}
