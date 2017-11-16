#pragma once

#include <iostream>
#include <time.h>

#define g_pRandom cRandom::GetInstance()
#define GETINT(num) cRandom::GetInstance()->getInt(num)
#define GETFROMINTTO(fromNum, toNum) cRandom::GetInstance()->getFromIntTo(fromNum, toNum)
#define GETFLOAT(Float) cRandom::GetInstance()->getFloat(Float)
#define GETFROMFLOATTO(fromFloat, toFloat) cRandom::GetInstance()->getFromFloatTo(fromFloat, toFloat)


class cRandom
{
	SINGLETONE(cRandom);
public:
	int getInt(int num);
	int getFromIntTo(int fromNum, int toNum);

	//rand 함수의 최대값 32767

	float getFloat(float num);

	float getFromFloatTo(float fromFloat, float toFloat);
};