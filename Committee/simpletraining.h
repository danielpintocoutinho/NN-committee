#ifndef __SIMPLETRAINING_H__
#define __SIMPLETRAINING_H__

#include <vector>
#include <tuple>

#include "../network.h"


template <class Type>
class SimpleTraining
{
public:
	void train(Type**, Type**, int, int, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	Type** answer(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int* cluster(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int getNumAnswers();
	int getNumClusters();
private:
	int numAnswers;
	int numClusters;
};


template <class Type>
void SimpleTraining<Type>::train(Type** input, Type** desired, int numInputs, int numAttributes, int numClasses, vector<Network<Type>*> networks,
	 vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	for (int i = 0; i < networks.size(); i++)
	{
		cout << endl << "Training network" << endl << endl;
		networks[i]->train(input, desired, numInputs);

	}
}

template <class Type>
Type** SimpleTraining<Type>::answer(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	this->numAnswers = networks.size();
	Type** answers = new Type*[networks.size()];
	for (int i = 0; i < networks.size(); i++)
		answers[i] = networks[i]->answer(input);
	return answers;

}

template <class Type>
int* SimpleTraining<Type>::cluster(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	int* clusters = new int[networks.size()];
	for (int i = 0; i < networks.size(); i++)
		clusters[i] = networks[i]->cluster(input);
	return clusters;

}

template <class Type>
int SimpleTraining<Type>::getNumAnswers()
{
	return this->numAnswers;
}



template <class Type>
int SimpleTraining<Type>::getNumClusters()
{
	return this->numClusters;
}

		


	


#endif