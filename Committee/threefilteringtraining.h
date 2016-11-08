#ifndef __THREEFILTERINGTRAINING_H__
#define __THREEFILTERINGTRAINING_H__

#include <vector>
#include <tuple>

#include "../network.h"


template <class Type>
class ThreeFilteringTraining
{
public:
	void setNumExamples(int);
	void train(Type**, Type**, int, int, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	Type** answer(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int* cluster(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int getNumAnswers();
	int getNumClusters();
private:
	int numExamples;
	bool classifiedCorrectly(Type*, Type*, int);
	int numAnswers;
	int numClusters;

};


template <class Type>
void ThreeFilteringTraining<Type>::setNumExamples(int numExamples)
{
	this->numExamples = numExamples;
}

template <class Type>
bool ThreeFilteringTraining<Type>::classifiedCorrectly(Type* obtained, Type* desired, int numClasses)
{
	//cout << "NumClasses: " << numClasses << endl;
	int labelObtained = 0, labelDesired = 0;
	Type maxLabelObtained = obtained[0], maxLabelDesired = desired[0];
	for (int i = 0; i < numClasses; i++)
	{
		//cout << desired[i] <<  " - " << obtained[i] << endl;
		if (maxLabelObtained < obtained[i])
		{
			labelObtained = i;
			maxLabelObtained = obtained[i];
		}

		if (maxLabelDesired < desired[i])
		{
			labelDesired = i;
			maxLabelDesired = desired[i];
		}
	}
	//cout << maxLabelDesired <<  " - " << maxLabelObtained << endl;
	//cout << labelDesired << " - " << labelObtained << endl;
	//cout << "Resultado: " << (labelObtained == labelDesired) << endl;


	return (labelObtained == labelDesired);
}

template <class Type>
void ThreeFilteringTraining<Type>::train(Type** input, Type** desired, int numInputs, int numAttributes, int numClasses, vector<Network<Type>*> networks,
	 vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	
	if (networks.size() == 3)
	{
		//cout << "Training" << endl;
		int indexInputSecond = numExamples-1;
		int indexInputThird;
		int numExamplesSecond = 0;
		int numExamplesThird = 0;
		bool coin;
		bool condition;
		Type** inputFirst;
		Type** inputSecond;
		Type** inputThird;
		Type** desiredFirst;
		Type** desiredSecond;
		Type** desiredThird;
		inputFirst = new Type*[numExamples];
		inputSecond = new Type*[numExamples];
		inputThird = new Type*[numExamples];
		desiredFirst = new Type*[numExamples];
		desiredSecond = new Type*[numExamples];
		desiredThird = new Type*[numExamples];
		for (int i = 0; i < numExamples; i++)
		{
			inputFirst[i] = new Type[numAttributes];
			inputSecond[i] = new Type[numAttributes];
			inputThird[i] = new Type[numAttributes];
			desiredFirst[i] = new Type[numAttributes];
			desiredSecond[i] = new Type[numAttributes];
			desiredThird[i] = new Type[numAttributes];
		}
		for (int i = 0; i < numExamples; i++)
		{
			for (int j = 0; j < numAttributes; j++)
				inputFirst[i][j] = input[i][j];
			for (int j = 0; j < numClasses; j++)
				desiredFirst[i][j] = desired[i][j];
		}

		networks[0]->train(inputFirst, desiredFirst, numExamples);
	//	cout << "Trained First" << endl;
		while(numExamplesSecond < numExamples)
		{
			coin = rand()%2;
			condition = true;
			if (coin) // Heads case
			{
				while(condition)
				{
					//cout << "Indice: " << indexInputSecond << endl;
					indexInputSecond++;
					Type* outputFirst = networks[0]->answer(input[indexInputSecond]);
					if (!classifiedCorrectly(outputFirst, desired[indexInputSecond], numClasses))
						condition = false;
					delete[] outputFirst;
				}
				
			}
			else // Tails case
			{
				while(condition)
				{
					//cout << "Indice: " << indexInputSecond << endl;
					indexInputSecond++;
					Type* outputFirst = networks[0]->answer(input[indexInputSecond]);
					if (classifiedCorrectly(outputFirst, desired[indexInputSecond], numClasses))
						condition = false;
					delete[] outputFirst;
				}
			}
			//cout << numExamplesSecond << " - " << indexInputSecond << endl;
			for (int i = 0; i < numAttributes; i++)
				inputSecond[numExamplesSecond][i] = input[indexInputSecond][i];
			for (int i = 0; i < numClasses; i++)
				desiredSecond[numExamplesSecond][i] = desired[indexInputSecond][i];
			numExamplesSecond++;
		}

		networks[1]->train(inputSecond, desiredSecond, numExamples);
		//cout << "Trained Second" << endl;
	//	cout << indexInputSecond << endl;
		indexInputThird = indexInputSecond;
		while(numExamplesThird < numExamples)
		{
			condition = true;
			while(condition)
			{
				//cout << "Indice: " << indexInputThird << endl;
				indexInputThird++;
				Type* outputFirst = networks[0]->answer(input[indexInputThird]);
				Type* outputSecond = networks[1]->answer(input[indexInputThird]);
				if (!(classifiedCorrectly(outputFirst, outputSecond, numClasses)))
					condition = false;

				delete[] outputFirst;
				delete[] outputSecond;
			}
			for (int i = 0; i < numAttributes; i++)
				inputThird[numExamplesThird][i] = input[indexInputThird][i];
			for (int i = 0; i < numClasses; i++)
				desiredThird[numExamplesThird][i] = desired[indexInputThird][i];
			numExamplesThird++;

		}

		networks[2]->train(inputThird, desiredThird, numExamples);
	//	cout << "Trained third" << endl;

	//	cout << indexInputThird << endl;

		
		for (int i = 0; i < numExamples; i++)
		{
			delete[] inputFirst[i];
			delete[] inputSecond[i];
			delete[] inputThird[i];
			delete[] desiredFirst[i];
			delete[] desiredSecond[i];
			delete[] desiredThird[i];
		}
		delete[] inputFirst;
		delete[] inputSecond;
		delete[] inputThird;
		delete[] desiredFirst;
		delete[] desiredSecond;
		delete[] desiredThird;



	}
}

template <class Type>
Type** ThreeFilteringTraining<Type>::answer(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{

	this->numAnswers = networks.size();
	Type** answers = new Type*[networks.size()];
	for (int i = 0; i < networks.size(); i++)
		answers[i] = networks[i]->answer(input);

	return answers;

}

template <class Type>
int* ThreeFilteringTraining<Type>::cluster(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	int* clusters = new int[networks.size()];
	for (int i = 0; i < networks.size(); i++)
		clusters[i] = networks[i]->cluster(input);
	return clusters;

}


template <class Type>
int ThreeFilteringTraining<Type>::getNumAnswers()
{
	return this->numAnswers;
}

template <class Type>
int ThreeFilteringTraining<Type>::getNumClusters()
{
	return this->numClusters;
}



	


#endif