#include "RNGUniform.h"

const int MOD1 = 30269;
const int MOD2 = 30307;
const int MOD3 = 30323;

const int MULT1 = 171;
const int MULT2 = 172;
const int MULT3 = 170;

RNGUniform::RNGUniform(unsigned iseed)
{
	setSeed(iseed);
}
void RNGUniform::setSeed(unsigned iseed)
{
	m1 = iseed % (MOD1 - 1) + 1;
	m2 = iseed % (MOD2 - 1) + 1;
	m3 = iseed % (MOD3 - 1) + 1;
    dvalue = 0;
	generate01();
}
double RNGUniform::generate01()
{
	double result = dvalue;
	nvAssert(result >= 0 && result <= 1);
	dvalue = (double)m1 / MOD1 + (double)m2 / MOD2 + (double)m3 / MOD3;
	m1 = (m1 * MULT1) % MOD1;
	m2 = (m2 * MULT2) % MOD2;
	m3 = (m3 * MULT3) % MOD3;
	dvalue -= (int)dvalue;
	return result;
}
