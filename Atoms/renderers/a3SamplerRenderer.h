﻿#ifndef A3_SAMPLERRENDERER_H
#define A3_SAMPLERRENDERER_H

#include <renderers/a3Renderer.h>

class a3Random;
class a3Sampler;
class a3Integrator;

class a3SamplerRenderer : public a3Renderer
{
public:
	a3SamplerRenderer();

    a3SamplerRenderer(int spp);

	~a3SamplerRenderer();

	void render(const a3Scene* scene);

    bool check();

    // 采样器
    a3Sampler* sampler;

    // 相机
    a3Sensor* camera;

    // 积分器
    a3Integrator* integrator;

    // 伽马校正开关
    bool enableGammaCorrection;

    // Samples Per Pixels
    int spp;
};

#endif 