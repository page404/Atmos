﻿#ifndef A3_RANDOM_H
#define A3_RANDOM_H

class a3Random
{
public:
    a3Random();

    void setFloatRange(float min, float max);

    void setIntRange(int min, int max);

    double randomDouble() const;

    // [0, 1)
    float randomFloat() const;

    // [0, 10]
    unsigned int randomInt() const;

private:
    class a3RandomPrivate;
    //a3RandomPrivate* random;
};

#endif