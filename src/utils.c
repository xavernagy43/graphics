#include "utils.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

int rand_int(int min, int max)
{
	return rand() * (max - min) + min;
}

float rand_float(float min, float max)
{
	float scale = rand() / (float)RAND_MAX;
	return min + scale * (max - min);
}