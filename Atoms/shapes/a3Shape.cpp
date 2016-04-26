﻿#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/log/a3Log.h>

a3Shape::a3Shape() :bsdf(NULL), bIsEmitter(false)
{
    emission.set(0, 0, 0);

    //type = A3_MATERIAL_NONE;

    // Glass
    refractiveIndex = 1.5f;
}

bool a3Shape::intersect(const a3Ray& ray, float* t) const
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

t3Vector3f a3Shape::getNormal(const t3Vector3f& hitPoint) const
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

bool a3Shape::hasBSDF() const
{ 
    return bsdf != NULL;
}

bool a3Shape::isEmitter() const
{
    return bIsEmitter;
}
 