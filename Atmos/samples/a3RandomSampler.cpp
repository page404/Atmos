﻿#include <samples/a3RandomSampler.h>
#include <core/random/a3Random.h>
#include <samples/a3CameraSample.h>

a3RandomSampler::a3RandomSampler()
{

}

int a3RandomSampler::getMoreSamples(int x, int y, a3CameraSample* sample, a3CameraSample *sampleFilter)
{
    sample->imageX = x;
    sample->imageY = y;

    if(sampleFilter)
    {
        // 在原有采样位置处[-1, 1]做随机采样
        sampleFilter->imageX = x + a3Random::randomFloat() * 2 - 1;
        sampleFilter->imageY = y + a3Random::randomFloat() * 2 - 1;
    }
    return 1;
}

 