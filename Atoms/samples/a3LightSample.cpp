﻿#include <samples/a3LightSample.h>
#include <core/random/a3Random.h>

a3LightSample::a3LightSample()
{
    static a3Random random;

    uPos[0] = random.randomFloat();

    uPos[1] = random.randomFloat();
}
 