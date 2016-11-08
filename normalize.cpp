#ifndef __NORMALIZE_H__
#define __NORMALIZE_H__


#include <cfloat>

#define INFINITE DBL_MAX

template<class Type>
void normalize(Type** inputSet, int lines, int columns)
{
	Type diference[columns];
	Type minimum[columns];
	for (int j = 0; j < columns; j++)
	{
		minimum[j] = INFINITE;
		Type maximum = 0;
		for (int i = 0; i < lines; i++)
		{
			if (inputSet[i][j] < minimum[j])
				minimum[j] = inputSet[i][j];
			if (inputSet[i][j] > maximum)
				maximum = inputSet[i][j];
		}
		diference[j] = maximum - minimum[j];
	}

	for (int j = 0; j < columns; j++)
	{
		if (diference[j] != 0)
			for (int i = 0; i < lines; i++)
			{
				inputSet[i][j] = (inputSet[i][j] - minimum[j]) /diference[j];
			}
	}
}


#endif