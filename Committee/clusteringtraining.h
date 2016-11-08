#ifndef __CLUSTERINGTRAINING_H__
#define __CLUSTERINGTRAINING_H__

#include <vector>
#include <tuple>

#include "../network.h"

template <class Type>
class ClusteringTraining
{
public:
	void train(Type**, Type**, int, int, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	Type** answer(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int getNumAnswers();
	int getNumClusters();
private:
	int numAnswers;
	int numClusters;
};


template <class Type>
void ClusteringTraining<Type>::train(Type** input, Type** desired, int numInputs, int numAttributes, int numClasses, vector<Network<Type>*> networks,
	 vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	vector<int> indexInputNetworks;
	for (int i = 0; i < networks.size(); i++)
		if (get<1>(connections[i]) == true)
			indexInputNetworks.push_back(i);

	for (int i = 0; i < indexInputNetworks.size(); i++)
		networks[indexInputNetworks[i]]->train(input, desired, numInputs);

	for (int i = 0; i < indexInputNetworks.size(); i++)
	{
		int currentNetwork = indexInputNetworks[i];
		vector< vector <int> > trainingClusters;
		for (int j = 0; j < get<0>(connections[currentNetwork]).size(); j++ )
		{
			vector<int> newTrainingCluster;
			trainingClusters.push_back(newTrainingCluster);
		}
		for (int j = 0; j < numInputs; j++)
		{
			int resultingCluster = networks[currentNetwork]->cluster(input[j]);
			trainingClusters[resultingCluster].push_back(j);
		}

		for (int j = 0; j < get<0>(connections[currentNetwork]).size(); j++)
		{
			int numTrainingInputs = trainingClusters[j].size();
			//cout << "Network: " << j << " | examples: " << numTrainingInputs << endl;
			if ( numTrainingInputs != 0)
			{

				Type** trainingInput = new Type*[numTrainingInputs];
				Type** trainingDesired = new Type*[numTrainingInputs];
				for (int k = 0; k < numTrainingInputs; k++)
				{
					trainingInput[k] = new Type[numAttributes];
					trainingDesired[k] = new Type[numAttributes];
				}
				for (int k = 0; k < numTrainingInputs; k++)
				{
					trainingInput[k] = input[trainingClusters[j][k]];
					trainingDesired[k] = desired[trainingClusters[j][k]];
				}
				get<0>(connections[currentNetwork])[j]->train(trainingInput, trainingDesired, numTrainingInputs);
			}
		}
	}

}

template <class Type>
Type** ClusteringTraining<Type>::answer(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple < vector<Network<Type>* >, bool, bool> > connections)
{
	vector<int> indexInputNetworks;
	for (int i = 0; i < networks.size(); i++)
		if (get<1>(connections[i]) == true)
			indexInputNetworks.push_back(i);
	this->numAnswers = indexInputNetworks.size();
	Type** answers = new Type*[this->numAnswers];
	for (int i = 0; i < this->numAnswers; i++)
	{
		int currentNetwork = indexInputNetworks[i];
		int answeringNetwork = networks[currentNetwork]->cluster(input);
		answers[i] = get<0>(connections[currentNetwork])[answeringNetwork]->answer(input);
	}
	return answers;
}


template <class Type>
int ClusteringTraining<Type>::getNumAnswers()
{
	return this->numAnswers;
}

template <class Type>
int ClusteringTraining<Type>::getNumClusters()
{
	return this->numClusters;
}

#endif