﻿#include <shapes/a3TriangleMesh.h> 


a3TriangleMesh::a3TriangleMesh():a3Shape("a3TriangleMesh")
{

}

t3Vector3f a3TriangleMesh::getNormal(const t3Vector3f& hitPoint, float u, float v) const
{
    return t3Vector3f::zero();
}

bool a3TriangleMesh::intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const
{
    return false;
}

