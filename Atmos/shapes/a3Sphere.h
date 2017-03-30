﻿#ifndef A3_SPHERE_H
#define A3_SPHERE_H

#include <shapes/a3Shape.h>

class a3Sphere : public a3Shape
{
public:
	a3Sphere(const t3Vector3f& center, const float radius = 0.0f);

    void set(const t3Vector3f& center, const float radius = 0.0f);

	bool intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    virtual void print() const;

    virtual float area() const;

    virtual t3Vector3f sample(const a3LightSample& sample) const;

	t3Vector3f center;

	float radius;
};

#endif