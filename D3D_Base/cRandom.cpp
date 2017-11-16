#include "stdafx.h"
#include "cRandom.h"

cRandom::cRandom()
{
	srand(GetTickCount());
}

cRandom::~cRandom()
{
}

int cRandom::getInt(int num)
{
	return rand() % num;
}

int cRandom::getFromIntTo(int fromNum, int toNum)
{
	return (rand() % (toNum - fromNum + 1)) + fromNum;
}

float cRandom::getFloat(float num)
{
	return ((float)rand() / (float)RAND_MAX) * num;
}

float cRandom::getFromFloatTo(float fromFloat, float toFloat)
{
	float rnd = (float)rand() / (float)RAND_MAX;
	return (rnd * (toFloat - fromFloat) + fromFloat);
}
