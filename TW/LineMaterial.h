#pragma once
#include "Material.h"
class LineMaterial :
    public Material
{
public:
    double EquivalentDiameter = 0;
    double EquivalentArea = 0;
    double WeightLength = 0;
};

