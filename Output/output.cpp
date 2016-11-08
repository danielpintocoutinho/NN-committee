#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <iostream>

#include "../Input/input.cpp"


using namespace std;

template <class Type>
class Output
{
public:
    Output();
    ~Output();
    void analyzeResult(Type**, Type**, int, int);
	double accuracy();
	void print();
private:
    Type* differenceResult;
	Type** expected;
	Type** obtained;
    int numResultInputs;
	int numClasses;
    
};

template <class Type>
Output<Type>::Output()
{
    this->numResultInputs = 0;
	this->numClasses = 0;
}

template <class Type>
Output<Type>::~Output()
{
    if (this->numResultInputs != 0)
	{
		if (this->numClasses != 0)
		{
			for (int i = 0; i < this->numResultInputs; i++)
			{
				delete[] this->expected[i];
				delete[] this->obtained[i];
			}
		}
		delete[] this->expected;
		delete[] this->obtained;
		delete[] this->differenceResult;
	}
	
}


template <class Type>
void Output<Type>::analyzeResult(Type** expected, Type** obtained, int numResultInputs, int numClasses)
{

//	cout << "ANALISANDO RESULTADO" << endl;

    this->numResultInputs = numResultInputs;
	this->numClasses = numClasses;
	
	this->expected = new Type*[this->numResultInputs];
	this->obtained = new Type*[this->numResultInputs];
    differenceResult = new Type[this->numResultInputs];
    for (int i = 0; i < numResultInputs; i++)
    {
		this->expected[i] = new Type[this->numClasses];
		this->obtained[i] = new Type[this->numClasses];
    }
    
    for (int i = 0; i < numResultInputs; i++)
	{
		for (int j = 0; j < numClasses; j++)
		{
			this->expected[i][j] = expected[i][j];
			this->obtained[i][j] = obtained[i][j];
		}
	}
   // cout << endl;
    for (int i = 0; i < numResultInputs; i++)
	{
		//cout << "Line " << i << endl << endl;
		int indexExpected = 0, indexObtained = 0;
		Type maxExpected = expected[i][0], maxObtained = obtained[i][0];
		for (int j = 0; j < numClasses; j++)
		{
			//cout << expected[i][j] << " > " << maxExpected << endl;
			if (expected[i][j] > maxExpected)
			{
				indexExpected = j;
				maxExpected = expected[i][j];
			}
			//cout << obtained[i][j] << " > " << maxObtained << endl;
			if (obtained[i][j] > maxObtained)
			{
				indexObtained = j;
				maxObtained = obtained[i][j];
			}
		}
		//cout  << endl << indexExpected << " == " << indexObtained << endl << endl;
		if (indexExpected == indexObtained)
			differenceResult[i] = 1;
		else
			differenceResult[i] = 0;
/*
		for (int j = 0; j < numClasses; j++)
		{
			cout << expected[i][j] << " ";
		}
		cout << endl;
		for (int j = 0; j < numClasses; j++)
		{
			cout << obtained[i][j] << " ";
		}
		cout << endl;

		cout << "indiceDesejado: " << indexExpected << " | indiceObtido: " << indexObtained << endl;*/
	}

    
}


template <class Type>
double Output<Type>::accuracy()
{
	double accuracyResult = 0, sumResult = 0;
	for (int i = 0; i < this->numResultInputs; i++)
	{
		sumResult += differenceResult[i];
	}
//	cout << sumResult << " / " << this->numResultInputs << endl; 
	accuracyResult = sumResult / this->numResultInputs;
	return accuracyResult;
}


template <class Type>
void Output<Type>::print()
{
	for (int i = 0; i < this->numResultInputs; i++)
		cout << differenceResult[i] << " ";
	cout << endl;
}


/*
int main()
{
	Output<double> outputinho;
	double** expectedinho;
	double** obtainedinho;
	int num = 5, classes = 3;
	
	expectedinho = new double*[num];
	obtainedinho = new double*[num];
	
	for (int i = 0; i < num; i++)
	{
		expectedinho[i] = new double[classes];
		obtainedinho[i] = new double[classes];
		for (int j = 0; j < classes; j++)
		{
			expectedinho[i][j] = 0;
			obtainedinho[i][j] = 0;
		}
	}	
	
	expectedinho[0][2] = 1;
	expectedinho[1][1] = 1;
	expectedinho[2][0] = 1;
	expectedinho[3][1] = 1;
	expectedinho[4][2] = 1;
	
	obtainedinho[0][1] = 0.4;
	obtainedinho[0][2] = 0.6;
	obtainedinho[1][0] = 1;
	obtainedinho[2][0] = 0.7;
	obtainedinho[3][0] = 1;
	obtainedinho[4][2] = 0.9;
	
	cout << "Expectedinho: " << endl;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < classes; j++)
		{
			cout << expectedinho[i][j] <<  " ";
		}
		cout << endl;
	}
	
	cout << "Obtainedinho: " << endl;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < classes; j++)
		{
			cout << obtainedinho[i][j] <<  " ";
		}
		cout << endl;
	}
	
	
	outputinho.analyzeResult(expectedinho, obtainedinho, num, classes);
	outputinho.print();
	cout << "Acuracia: " << outputinho.accuracy() << endl;
	return 0;
	
}

*/

#endif