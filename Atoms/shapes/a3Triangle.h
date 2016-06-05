﻿#ifndef A3_TRIANGLE_H
#define A3_TRIANGLE_H

#include <shapes/a3Shape.h>

class a3Triangle:public a3Shape
{
public:
    a3Triangle();

    a3Triangle(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2);

    void set(t3Vector3f v0, t3Vector3f v1, t3Vector3f v2);

    void setVertexNormal(t3Vector3f n0, t3Vector3f n1, t3Vector3f n2);

    void setTextureCoordinate(t3Vector3f vt0, t3Vector3f vt1, t3Vector3f vt2);

    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    virtual float area() const;

    virtual t3Vector3f sample(const a3LightSample& sample) const;

    t3Vector3f v0, v1, v2;

    // 顶点法线
    t3Vector3f n0, n1, n2;

    // 顶点纹理坐标
    t3Vector3f vt0, vt1, vt2;

    bool bEnableBackfaceCulling;
};

#endif 