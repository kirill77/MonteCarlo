#pragma once

#include <math.h>
#include <basics/vectors.h>

struct DistributionsTest
{
    static void test();

protected:
    static bool s_bSphereVolumeTestPassed, s_bSphereSurfaceTestPassed;
private:
    static void testSphereVolumeDistribution();
    static void testSphereSurfaceDistribution();
};

template <class T>
struct SphereVolumeDistribution : public DistributionsTest
{
    // generates point inside sphere located at (0,0,0), with radius=1
    static rtvector<T, 3> generate(const rtvector<T, 3>& in01)
    {
        nvAssert(s_bSphereVolumeTestPassed);
        nvAssert(in01[0] >= 0 && in01[0] <= 1);
        nvAssert(in01[1] >= 0 && in01[1] <= 1);
        nvAssert(in01[2] >= 0 && in01[2] <= 1);
        T phi = in01[0] * 2 * M_PI;
        T cosTheta = in01[1] * 2 - 1;
        T sinTheta = sqrt(1 - sqr(cosTheta));
        T r = pow(in01[2], 1. / 3);
        rtvector<T, 3> p({ r * sinTheta * cos(phi), r * sinTheta * sin(phi), r * cosTheta });
        nvAssert(aboutEqual(lengthSquared(p), sqr(r)));
        return p;
    }
};

template <class T>
struct SphereSurfaceDistribution : public DistributionsTest
{
    // generates point on sphere located at (0,0,0), with radius=1
    static rtvector<T, 3> generate(const rtvector<T, 2>& in01)
    {
        nvAssert(s_bSphereSurfaceTestPassed);
        return makeVector<T, 3>(0);
    }
};