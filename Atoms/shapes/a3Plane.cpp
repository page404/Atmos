﻿#include <shapes/a3Plane.h>
#include <core./a3Ray.h>
#include <core/a3Warp.h>
#include <samples/a3LightSample.h>

#define A3_PLANE_COMPOSEDOF_TRIANGLES

a3Plane::a3Plane(const t3Vector3f& p, const t3Vector3f& normal, float width, float height) 
    :p(p), normal(normal.getNormalized()), width(width), height(height), a3Shape("a3Plane")
{
#ifndef A3_PLANE_COMPOSEDOF_TRIANGLES
    aabb.set(t3Vector3f(-A3_INFINITY), t3Vector3f(A3_INFINITY));
#else
    // 初始化三角形序列
    init();

    aabb = a3AABB::calUnion(a.aabb, b.aabb);
#endif
}


void a3Plane::set(const t3Vector3f& p, const t3Vector3f& normal, float width, float height)
{
    this->p = p;
    this->normal = normal.getNormalized();

#ifndef A3_PLANE_COMPOSEDOF_TRIANGLES
    aabb.set(t3Vector3f(-A3_INFINITY), t3Vector3f(A3_INFINITY));
#else
    // 初始化三角形序列
    init();

    aabb = a3AABB::calUnion(a.aabb, b.aabb);
#endif
}

void a3Plane::init()
{
    // shape坐标系
    a.set(t3Vector3f(-width / 2.f, height / 2.f, 0.f),
          t3Vector3f(-width / 2.f, -height / 2.f, 0.f),
          t3Vector3f(width / 2.f, height / 2.f, 0.f));

    b.set(t3Vector3f(width / 2.f, -height / 2.f, 0.f),
          t3Vector3f(width / 2.f, height / 2.f, 0.f),
          t3Vector3f(-width / 2.f, -height / 2.f, 0.f));

    // 纹理坐标
    a.setTextureCoordinate(t3Vector3f(0.f, 0.f, 0.f),
                           t3Vector3f(0.f, 1.f, 0.f),
                           t3Vector3f(1.f, 0.f, 0.f));

    b.setTextureCoordinate(t3Vector3f(1.f, 1.f, 0.f),
                           t3Vector3f(1.f, 0.f, 0.f),
                           t3Vector3f(0.f, 1.f, 0.f));

    // 顶点法线
    a.setVertexNormal(t3Vector3f(0.f, 0.f, 1.f),
                      t3Vector3f(0.f, 0.f, 1.f),
                      t3Vector3f(0.f, 0.f, 1.f));

    b.setVertexNormal(t3Vector3f(0.f, 0.f, 1.f),
                      t3Vector3f(0.f, 0.f, 1.f),
                      t3Vector3f(0.f, 0.f, 1.f));

    // 以normal为正向上的局部坐标系
    t3Vector3f nx, ny;
    a3OrthonomalSystem(normal, nx, ny); 
    // plane to world
    worldToPlane._mat[0].set(ny.x, normal.x, nx.x, 0.f);
    worldToPlane._mat[1].set(ny.y, normal.y, nx.y, 0.f);
    worldToPlane._mat[2].set(ny.z, normal.z, nx.z, 0.f);
    worldToPlane._mat[3].set(0.f, 0.f, 0.f, 1.f);
    // world to plane
    //worldToPlane = worldToPlane.getInverse();
}


t3Vector3f a3Plane::sample(const a3LightSample& sample) const
{
    // 随机选取其中一个三角形
    if(sample.uPos[0] > 0.5f)
        return a.sample(sample);
    else
        return b.sample(sample);
}

float a3Plane::area() const
{
    return width * height;
}

bool a3Plane::intersect(const a3Ray& ray, float* _t, float* _u, float* _v, float* _vtu, float* _vtv) const
{
#ifndef A3_PLANE_COMPOSEDOF_TRIANGLES
    // 规定与射线方向相对时可见 因此需要反转normal
    // 判断圆盘与直线平行关系
    t3Vector3d _normal(-normal), direction(ray.direction);

    double denominator = (_normal).dot(direction), tHit = 0.0;

    // 无限远平面不适用双面
    //denominator = t3Math::Abs(denominator);

    if(denominator > A3_TOLERANCE_DOUBLE)
    {
        t3Vector3d dir(p - ray.origin);

        tHit = (dir.dot(_normal) / denominator);

        if(tHit > ray.minT && tHit < ray.maxT)
        {
            *t = tHit;
            return true;
        }
    }

    return false;
#else
    // world to plane
    t3Vector3f d = ray.direction * worldToPlane;
    t3Vector3f o = ray.origin * worldToPlane + p;

    a3Ray _ray(o, d, A3_RAY_BIAS);

    bool intersected = false;

    float t = 0.f, u = 0.f, v = 0.f, vtu = 0.f, vtv = 0.f;
    if(a.intersect(_ray, &t, &u, &v, &vtu, &vtv))
        intersected = true;
    else if(b.intersect(_ray, &t, &u, &v, &vtu, &vtv))
        intersected = true;

    *_t = t;
    *_u = u;
    *_v = v;
    *_vtu = vtu;
    *_vtv = vtv;

    return intersected;
#endif
}

t3Vector3f a3Plane::getNormal(const t3Vector3f& hitPoint, float u, float v) const
{
    return normal;
}
 