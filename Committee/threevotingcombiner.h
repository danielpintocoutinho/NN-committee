#ifndef __THREEVOTINGCOMBINER_H__
#define __THREEVOTINGCOMBINER_H__

#include <iostream>

using namespace std;

template <class Type>
class ThreeVotingCombiner
{
public:
	Type* answer(Type**, int, int);
	int cluster(int*, int);
};


template <class Type>
Type* ThreeVotingCombiner<Type>::answer(Type** answers, int numNetworks, int numClasses)
{
	if (numNetworks == 3)
	{
		Type* networkOutput = new Type[numClasses];

		int firstLabel = 0, secondLabel = 0, thirdLabel;
		Type maxFirstLabel = answers[0][0], maxSecondLabel = answers[1][0], maxThirdLabel = answers[2][0];
		for (int i = 1; i < numClasses; i++)
		{
			if (answers[0][i] > maxFirstLabel)
			{
				firstLabel = i;
				maxFirstLabel = answers[0][i];
			}
			if (answers[1][i] > maxSecondLabel)
			{
				secondLabel = i;
				maxSecondLabel = answers[1][i];
			}
			if (answers[2][i] > maxThirdLabel)
			{
				thirdLabel = i;
				maxThirdLabel = answers[2][i];
			}

		}
		if (firstLabel == secondLabel)
			for (int i = 0; i < numClasses; i++)
			 	networkOutput[i] = answers[1][i];

		else
			for (int i = 0; i < numClasses; i++)
				networkOutput[i] = answers[2][i];

		return networkOutput;
	}

}

template <class Type>
int ThreeVotingCombiner<Type>::cluster(int* clusters, int numClusters)
{

}

#endif