#ifndef __AVERAGECOMBINER_H__
#define __AVERAGECOMBINER_H__

#include <iostream>

using namespace std;

template <class Type>
class AverageCombiner
{
public:
	Type* answer(Type**, int, int);
	int cluster(int*, int);
};


template <class Type>
Type* AverageCombiner<Type>::answer(Type** answers, int numNetworks, int numClasses)
{
	//cout << "___________Combiner Output___________" << endl;
	Type* networkOutput = new Type[numClasses];
	for (int i = 0; i < numClasses; i++)
		networkOutput[i] = 0;

	for (int i = 0; i < numNetworks; i++)
	{
		for (int j = 0; j < numClasses; j++)
		{
			networkOutput[j] += answers[i][j];
		//	cout << answers[i][j] << " ";
		}
		//cout << endl;
	}

	//cout << "Result: " << endl;
	for (int i = 0; i < numClasses; i++)
	{
		networkOutput[i] /= numNetworks;
		//cout << networkOutput[i] << " ";
	}
	//cout << endl;


	return networkOutput;
}

template <class Type>
int AverageCombiner<Type>::cluster(int* clusters, int numClusters)
{

}

#endif