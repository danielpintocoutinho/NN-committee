#ifndef __GENERALVOTINGCOMBINER_H__
#define __GENERALVOTINGCOMBINER_H__

#include <iostream>

using namespace std;

template <class Type>
class GeneralVotingCombiner
{
public:
	Type* answer(Type**, int, int);
	int cluster(int*, int);
};


template <class Type>
Type* GeneralVotingCombiner<Type>::answer(Type** answers, int numAnswers, int numClasses)
{
	int votes[numClasses];
	Type* answer = new Type[numClasses];
	for (int i = 0; i < numClasses; i++)
		votes[i] = 0;
	int label;
	Type maxLabel;
	for (int i = 0; i < numAnswers; i++)
	{
		label = 0;
		maxLabel = answers[i][0];
		for (int j = 0; j < numClasses; j++)
			if (maxLabel < answers[i][j])
			{
				label = j;
				maxLabel = answers[i][j];
			}
		votes[label]++;
	}

	label = 0;
	int maxVote = votes[0];
	for (int i = 1; i < numClasses; i++)
		if (maxVote < votes[i])
		{
			label = i;
			maxVote = votes[i];
		}

	for (int i = 0; i < numClasses; i++)
		if (i != label)
			answer[i] = 0;
		else
			answer[i] = 1;
	return answer;
}

template <class Type>
int GeneralVotingCombiner<Type>::cluster(int* clusters, int numClusters)
{

}

#endif