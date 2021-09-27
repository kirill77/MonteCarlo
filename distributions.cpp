#include <chrono>
#include "distributions.h"
#include "basics/bspheres.h"
#include "RNGUniform.h"
#include "RNGSobol.h"

void DistributionsTest::testSphereVolumeDistribution()
{
    const NvU32 N_POINTS_PER_DIM = 32;
    const NvU32 N_SPHERES = 32;
    const NvU32 N_RANDOM_NUMBERS = N_SPHERES * 3;

    // select sphere radius to get around 64=(4*4*4) points in there
    static double fSphereRad = (4. / N_POINTS_PER_DIM);

    auto nSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    RNGUniform rng((NvU32)time(nullptr));

    // generate spheres of given radius at random location so that they are fully inside unit sphere
    BSphereD spheres[N_SPHERES];
    NvU32 nSpherePoints[N_SPHERES] = { 0 };
    for (NvU32 uSphere = 0; uSphere < N_SPHERES; ++uSphere)
    {
        spheres[uSphere].m_fRad = fSphereRad;
        spheres[uSphere].m_vCenter[0] = rng.generate01() * 2 - 1;
        spheres[uSphere].m_vCenter[1] = rng.generate01() * 2 - 1;
        spheres[uSphere].m_vCenter[2] = rng.generate01() * 2 - 1;

        // our small sphere must be inside unit sphere, so shift it towards 0,0,0 if needed
        double fCurDist = length(spheres[uSphere].m_vCenter);
        double fFarthestPoint = fCurDist + spheres[uSphere].m_fRad;
        if (fFarthestPoint > 1)
        {
            auto vShiftVector = spheres[uSphere].m_vCenter / -fCurDist;
            spheres[uSphere].m_vCenter += vShiftVector * (fFarthestPoint - 1);
            fCurDist = length(spheres[uSphere].m_vCenter);
            fFarthestPoint = fCurDist + spheres[uSphere].m_fRad;
            nvAssert(aboutEqual(fFarthestPoint, 1.));
        }
    }

    s_bSphereVolumeTestPassed = true; // this is temporary to avoid assert firing

    // generate uniformly distributed points and count how many does each sphere get
    RNGSobol sobol;
    sobol.setSeed(N_POINTS_PER_DIM * N_POINTS_PER_DIM * N_POINTS_PER_DIM);
    for (NvU32 uPoint = 0; uPoint < N_POINTS_PER_DIM * N_POINTS_PER_DIM * N_POINTS_PER_DIM; ++uPoint)
    {
        double3 p;
        p[0] = sobol.generate01();
        p[1] = sobol.generate01();
        p[2] = sobol.generate01();
        double3 outP = SphereVolumeDistribution<double>::generate(p);
        for (NvU32 uSphere = 0; uSphere < N_SPHERES; ++uSphere)
        {
            if (spheres[uSphere].intersects(outP))
            {
                ++nSpherePoints[uSphere];
            }
        }
        sobol.nextSeed();
    }

    // check how many points we got in each sphere
    double nTotalSpheres = 0;
    for (NvU32 uSphere = 0; uSphere < N_SPHERES; ++uSphere)
    {
        nTotalSpheres += nSpherePoints[uSphere];
        if (nSpherePoints[uSphere] < 53 || nSpherePoints[uSphere] > 76)
        {
            nvAssert(false);
            s_bSphereVolumeTestPassed = false;
            return;
        }
    }

    // check how many points we got per sphere on average
    nTotalSpheres /= N_SPHERES;
    if (nTotalSpheres < 62 || nTotalSpheres > 65.5)
    {
        nvAssert(false);
        s_bSphereVolumeTestPassed = false;
    }
}

void DistributionsTest::testSphereSurfaceDistribution()
{
}

void DistributionsTest::test()
{
    testSphereVolumeDistribution();
}

bool DistributionsTest::s_bSphereVolumeTestPassed = false;

bool DistributionsTest::s_bSphereSurfaceTestPassed = false;