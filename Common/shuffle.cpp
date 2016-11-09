#ifndef __SHUFFLE_H__
#define __SHUFFLE_H__

template<class Type>
void shuffle(Type** matrix, int lines, int columns)
{
	int firstPosition = 0;
	int secondPosition = 0;
	for (int i = 0; i < lines; i++)
	{
		while (firstPosition == secondPosition)
		{
			firstPosition = rand()%lines;
			secondPosition = rand()%lines;
		}
		Type aux[columns];
		for (int j = 0; j < columns; j++)
			aux[j] = matrix[firstPosition][j];
		for (int j = 0; j < columns; j++)
			matrix[firstPosition][j] = matrix[secondPosition][j];
		
		for (int j = 0; j < columns; j++)
			matrix[secondPosition][j] = aux[j];
		secondPosition = firstPosition;
	}
}

template<class Type>
void shuffle(Type** firstMatrix, Type** secondMatrix, int lines, int columns)
{
	int firstPosition = 0;
	int secondPosition = 0;
	for (int i = 0; i < lines; i++)
	{
		while (firstPosition == secondPosition)
		{
			firstPosition = rand()%lines;
			secondPosition = rand()%lines;
		}
		Type aux[columns];
		for (int j = 0; j < columns; j++)
			aux[j] = firstMatrix[firstPosition][j];
		
		for (int j = 0; j < columns; j++)
			firstMatrix[firstPosition][j] = firstMatrix[secondPosition][j];
		
		for (int j = 0; j < columns; j++)
			firstMatrix[secondPosition][j] = aux[j];
		
		
		
		for (int j = 0; j < columns; j++)
			aux[j] = secondMatrix[firstPosition][j];
		
		for (int j = 0; j < columns; j++)
			secondMatrix[firstPosition][j] = secondMatrix[secondPosition][j];
		
		for (int j = 0; j < columns; j++)
			secondMatrix[secondPosition][j] = aux[j];	
		
		secondPosition = firstPosition;
	}
}


#endif