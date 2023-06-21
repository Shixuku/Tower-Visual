#pragma once
#include"Node.h"
#include <vtkRenderer.h>
class Constrained_image
{
public:
	void Draw_fixed_Constrained(double x, double y, double z,vtkRenderer* renderer);
	void Draw_hinge_joint(double x, double y, double z, vtkRenderer* renderer);
};

