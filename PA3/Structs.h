#pragma once

#include <cmath>

struct IdPair
{
    int vertexId0;
    int vertexId1;
};

struct Double3
{
    double x, y, z;

    static double Distance(Double3 left, Double3 right);
};

inline
double Double3::Distance(Double3 left, Double3 right)
{
    double xx = (left.x-right.x)*(left.x-right.x);
    double yy = (left.y-right.y)*(left.y-right.y);
    double zz = (left.z-right.z)*(left.z-right.z);
    return sqrt( xx + yy + zz );
}