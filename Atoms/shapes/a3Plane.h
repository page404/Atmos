#ifndef A3_PLANE_H
#define A3_PLANE_H

#include <shapes/a3Shape.h>

// ����
class a3Plane:public a3Shape
{
public:
    a3Plane(const t3Vector3f& p, const t3Vector3f& normal);

    virtual float intersect(const a3Ray& ray) const;

    virtual t3Vector3f getNormal(const t3Vector3f& vector) const;

    // ƽ����һ�㼰�䳯��
    t3Vector3f p, normal;
};

#endif