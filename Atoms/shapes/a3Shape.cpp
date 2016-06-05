﻿#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/log/a3Log.h>

a3Shape::a3Shape() :bsdf(NULL), bIsEmitter(false), bUseFaceNormal(false), bCalTextureCoordinate(true), name("a3Shape")
{
    emission.set(0, 0, 0);

    //type = A3_MATERIAL_NONE;

    // Glass
    refractiveIndex = 1.5f;
}


a3Shape::a3Shape(std::string name):bsdf(NULL), bIsEmitter(false), bUseFaceNormal(false), bCalTextureCoordinate(true), name(name)
{
    emission.set(0, 0, 0);

    // Glass
    refractiveIndex = 1.5f;
}

bool a3Shape::intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const
{
	a3Log::warning("Unimplemented a3Shape::intersect() method called");

	return 0.0f;
}

//t3Vector3f a3Shape::sample(float u1, float u2) const
//{
//    a3Log::warning("Unimplemented a3Shape::sample() method called");
//
//    return t3Vector3f::zero();
//}

t3Vector3f a3Shape::getNormal(const t3Vector3f& hitPoint, float u, float v) const
{
    a3Log::warning("Unimplemented a3Shape::sample() method called");

    return t3Vector3f::zero();
}

//void a3Shape::setMaterial(const t3Vector3f color, const t3Vector3f emission, int type)
//{
//	this->color = color;
//	this->emission = emission;
//	//this->type = type;
//}

void a3Shape::print() const
{
#ifdef _DEBUG
    emission.print("a3Shape.emission:");
    //std::cout << ", type:" << type << std::endl;
#endif 
}

const a3BSDF* a3Shape::getBSDF() const
{
    return bsdf;
}

a3BSDF* a3Shape::getBSDF()
{
    return bsdf;
}

void a3Shape::setBSDF(a3BSDF* bsdf)
{
    this->bsdf = bsdf;
}


float a3Shape::area() const
{
    a3Log::warning("Unimplemented a3Shape::area() method called");

    return 0.0f;
}


float a3Shape::pdf(const t3Vector3f& p, const t3Vector3f& wi)
{
    a3Ray ray(p, wi);
    float t, u, v, vtu, vtv;
    if(!intersect(ray, &t, &u, &v, &vtu, &vtv))
        return 0.0f;

    t3Vector3f hitPoint = ray(t);

    float cosTheta = getNormal(hitPoint, u, v).dot(-wi);

    // 对面积的采样转换为立体角
    float pdf = (p - hitPoint).lengthSquared() / (cosTheta * area());

    return _finite(pdf) ? 0.0f : pdf;
}


t3Vector3f a3Shape::sample(const a3LightSample& sample) const
{
    a3Log::warning("Unimplemented a3Shape::sample() method called");

    return t3Vector3f::zero();
}


a3AreaLight* a3Shape::getAreaLight() const
{
    return areaLight;
}

bool a3Shape::hasBSDF() const
{ 
    return bsdf != NULL;
}

bool a3Shape::isEmitter() const
{
    return bIsEmitter;
}

 