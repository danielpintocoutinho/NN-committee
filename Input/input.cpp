#ifndef __INPUT_H__
#define __INPUT_H__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * A class that will be responsible for receiving the data from a text file and saving it in a matrix;
 * 
 */
template <class Type>
class Input
{
public:
	Input();
	~Input();
	void readTrainingData(const char*, bool);
	void readTestData(const char*, bool);
	Type** getTrainingData();
	Type** getTestData();
	Type** getTrainingclasses();
	void printTrain();
	void printTrainClasses();
	void printTest();
	void printTestClasses();

private:
	ifstream inputData;
	Type** trainingData;
	Type** testData;
	Type** trainingClasses;
	Type** testClasses;
	int numAttributes;
	int numClasses;
	int numTrainingInputs;
	int numTestInputs;
};

template <class Type>
Input<Type>::Input()
{
	this->numAttributes = 0;
	this->numClasses = 0;
	this->numTestInputs = 0;
	this->numTrainingInputs = 0;
}


template <class Type>
Input<Type>::~Input()
{
	if (this->numTrainingInputs != 0)
	{
		for (int i = 0; i < this->numTrainingInputs; i++)
		{
			delete[] this->trainingData[i];
		}
		delete[] this->trainingData;
	}
	
	if (this->numClasses != 0)
	{
		for (int i = 0; i < this->numTrainingInputs; i++)
		{
			delete[] this->trainingClasses[i];
		}
		delete[] this->trainingClasses;
	}
	
	if (this->numTestInputs != 0)
	{
		for (int i = 0; i < this->numTestInputs; i++)
			delete[] this->testData[i];
		delete[] this->testData;
	}
	
}



template <class Type>
void Input<Type>::readTrainingData(const char* filename, bool supervised)
{
	ifstream dataFile(filename);
	dataFile >> numTrainingInputs >> numAttributes;
	vector<string> classes;
	trainingData = new Type* [numTrainingInputs];
	Type temporaryClasses[numTrainingInputs];
	for (int i = 0; i < numTrainingInputs; i++)
		trainingData[i] = new Type[numAttributes];
	
	for (int i = 0; i < numTrainingInputs; i++)
	{
		for (int j = 0; j < numAttributes; j++)
			dataFile >> trainingData[i][j];
		if (supervised)
		{
			string temporaryClass;
			dataFile >> temporaryClass;
			
			bool inClass = false;
			int indexInClass;
			int j;
			for (j = 0; j < classes.size(); j++)
				if (classes[j] == temporaryClass)
				{
					inClass = true;
					indexInClass = j;
					break;
				}
			if (inClass == false)
			{
				classes.push_back(temporaryClass);
			}
			temporaryClasses[i] = j;
		}
	}
	if (supervised)
	{
		this->numClasses = classes.size();
		trainingClasses = new Type*[numTrainingInputs];
		for (int i = 0; i < numTrainingInputs; i++)
		{
			trainingClasses[i] = new Type [numClasses];
		}
		for (int i = 0; i < numTrainingInputs; i++)
		{
			for (int j = 0; j < numClasses; j++)
				trainingClasses[i][j] = 0;
			int classIndex = temporaryClasses[i];
			trainingClasses[i][classIndex] = 1;
		}
	}
}

template <class Type>
void Input<Type>::readTestData(const char* filename, bool supervised)
{
	ifstream dataFile(filename);
	dataFile >> numTestInputs;
	vector<string> classes;
	testData = new Type* [numTestInputs];
	Type temporaryClasses[numTestInputs];
	for (int i = 0; i < numTestInputs; i++)
		testData[i] = new Type[numAttributes];
	
	for (int i = 0; i < numTestInputs; i++)
	{
		for (int j = 0; j < numAttributes; j++)
			dataFile >> testData[i][j];
		if (supervised)
		{
			string temporaryClass;
			dataFile >> temporaryClass;
			
			bool inClass = false;
			int indexInClass;
			int j;
			for (j = 0; j < classes.size(); j++)
				if (classes[j] == temporaryClass)
				{
					inClass = true;
					indexInClass = j;
					break;
				}
			if (inClass == false)
			{
				classes.push_back(temporaryClass);
			}
			temporaryClasses[i] = j;
		}
	}
	if (supervised)
	{
		testClasses = new Type*[numTestInputs];
		for (int i = 0; i < numTestInputs; i++)
		{
			testClasses[i] = new Type [numClasses];
		}
		for (int i = 0; i < numTestInputs; i++)
		{
			for (int j = 0; j < numClasses; j++)
				testClasses[i][j] = 0;
			int classIndex = temporaryClasses[i];
			testClasses[i][classIndex] = 1;
		}
	}
}



template <class Type>
Type** Input<Type>::getTrainingData()
{
	return this->trainingData;
}

template <class Type>
Type** Input<Type>::getTestData()
{
	return this->testData;
}

template <class Type>
Type** Input<Type>::getTrainingclasses()
{
	return this->trainingClasses;
}



template <class Type>
void Input<Type>::printTrain()
{
	for (int i = 0; i < this->numTrainingInputs; i++)
	{
		for (int j = 0; j < this->numAttributes; j++)
			cout << trainingData[i][j] << " ";
		cout << endl;
	}
}

template <class Type>
void Input<Type>::printTrainClasses()
{
	for (int i = 0; i < this->numTrainingInputs; i++)
	{
		for (int j = 0; j < this->numClasses; j++)
			cout << trainingClasses[i][j] << " ";
		cout << endl;
	}
}

template <class Type>
void Input<Type>::printTest()
{
	for (int i = 0; i < this->numTestInputs; i++)
	{
		for (int j = 0; j < this->numAttributes; j++)
			cout << testData[i][j] << " ";
		cout << endl;
	}
}

template <class Type>
void Input<Type>::printTestClasses()
{
	for (int i = 0; i < this->numTestInputs; i++)
	{
		for (int j = 0; j < this->numClasses; j++)
			cout << testClasses[i][j] << " ";
		cout << endl;
	}
}


/*
int main()
{
	string a("iris1"), b("iris2"), c("iris3");
	//cout << (a == b) << endl;
	vector <string> classes;
	classes.push_back(a);
	classes.push_back(b);
	cout << classes.size() << endl;
	int i;
	for (i = 0; i < classes.size(); i++)
	{
		cout << "Igual: " << (classes[i] == c) << endl;
	}
	classes.push_back(c);
	cout << "Tamanho: " << classes.size() << " - i: " << i << endl;
	
	Input<double> littleInput;
	littleInput.readTrainingData("../Datasets/iris.data", true);
	littleInput.printTrain();
	littleInput.printTrainClasses();
	return 0;
}
*/


#endif
