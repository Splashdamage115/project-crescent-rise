#pragma once

#include "Library.h"

class MinMax {
public:
	float getMin() { return min; }
	float getMax() { return max; }

	void AddValue(float v)
	{
		if (firstPass)
		{
			min = v;
			max = v;
			firstPass = false;
		}

		if (v > max)
		{
			max = v;
		}
		if (v < min)
		{
			min = v;
		}
	}
private:
	float min = FLT_MAX;
	float max = FLT_MIN;
	bool firstPass = true; 
};