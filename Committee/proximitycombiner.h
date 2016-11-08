#ifndef __PROXIMITYCOMBINER_H__
#define __PROXIMITYCOMBINER_H__

#include <iostream>

using namespace std;

template <class Type>
class ProximityCombiner
{
public:
	Type* answer(Type**, int, int);
	int cluster(int*, int);
};


template <class Type>
Type* ProximityCombiner<Type>::answer(Type** answers, int numNetworks, int numClasses)
{
	/*
	cout << "___________Combiner Output___________" << endl;
	for (int i = 0; i < numNetworks; i++)
	{
		for (int j = 0; j < numClasses; j++)
			cout << answers[i][j] << " ";
		cout << endl;
	}*/
	Type* networkOutput = new Type[numClasses];

	Type minimumProximity = 1;
	int indexMinimumProximity = 0;
	for (int i = 0; i < numNetworks; i++)
	{
		//cout << "Network: " << i  << endl;
		Type currentProximity = 1;
		for (int j = 0; j < numClasses; j++)
		{
			Type proximityValue = 1 - answers[i][j];
		//	cout << "proximityValue = " << 1 << " - " << answers[i][j] << " == " << (1 - answers[i][j]) << endl;
			if (proximityValue < currentProximity)
				currentProximity = proximityValue;
		}
		if (currentProximity < minimumProximity)
		{
			minimumProximity = currentProximity;
			indexMinimumProximity = i;
		}
	}
	//cout << "Indice escolhido: " << indexMinimumProximity << " | proximity: " << minimumProximity << endl;
	for (int i = 0; i < numClasses; i++)
		networkOutput[i] = answers[indexMinimumProximity][i];

	return networkOutput;
}

template <class Type>
int ProximityCombiner<Type>::cluster(int* clusters, int numClusters)
{

}

#endif