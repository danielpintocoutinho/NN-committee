#ifndef __LOGISTICACTIVATIONFUNCTION_H__
#define __LOGISTICACTIVATIONFUNCTION_H__

#include <cmath>
#include "activationfunction.h"

/**
 * Derived class from ActivationFunction that implements a linear function.
 */
template <class Type>
class LogisticActivationFunction : public ActivationFunction<Type>
{
	
public:
	
	/**
	 * Method that implements the linear function between two arrays.
	 * The formula is given by:
	 * \f$ linear(weights, input, arrayLength) = 1 \f$
	 * @param weights First array of weights
	 * @param input Second array of weights
	 * @param arrayLength Length of the arrays
	 * @param threshold 
	 */
	void exec(Type value, Type threshold)
	{
		//cout << "value: " << value << " - threshold: " << threshold << endl;

		this->result = 1;
		Type denominator = 1 + exp(-value);
		this->result /= denominator;
		//cout << "den: " << denominator << " | res: " << this->result << endl;
		
		/*
		 * this->result = value; direto?
		 */
		
	}
	
	void derived(Type value, Type threshold)
	{
		this->derivedResult = this->result * (1 - this->result);
	}
	
};



#endif
