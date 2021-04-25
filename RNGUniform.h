#pragma once

#include <algorithm>
#include <basics/mybasics.h>

/// generator of uniformly distributed pseudo random numbers
class RNGUniform
{
public:
	/// default constructor
	RNGUniform(unsigned uSeed = 9973);
	/// set the seed
	void setSeed(unsigned uSeed);
	/// generate pseudo-random number >= 0 and < 1
	double generate01();
	inline unsigned getNDims() const
	{
		return 1;
	}
	inline void nextSeed() { }
	inline void setDim(unsigned) { }

	// generate unsigned integer value >= uMin and < uMax
	inline unsigned generateUnsigned(unsigned uMin, unsigned nValues)
	{
        nvAssert(nValues > 0);
        NvU32 uValue = (unsigned)(nValues * generate01());
		nvAssert(uValue < nValues);
		return uMin + uValue;
	}
	// generate double value >= fMin and < fMax;
	inline double generateBetween(double fMin, double fMax)
	{
		double fTmp = generate01();
		return fMin + (fMax - fMin) * fTmp;
	}
    inline bool generateBool()
    {
        return generate01() < 0.5;
    }
	template <class T>
	inline NvU32 choooseInSortedArray(const T *pValues, NvU32 nValues)
	{
		nvAssert(pValues[0] > 0);
		double randomValue = generateBetween(0, pValues[nValues - 1]);
		return binarySearch(pValues, nValues, randomValue);
	}

private:
	double dvalue;
	int m1, m2, m3;
};
