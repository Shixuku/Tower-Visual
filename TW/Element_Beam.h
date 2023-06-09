#pragma once
#include "Element.h"
class Element_Beam :public Element
{
public:
	double direction[3];//Áºn1·½Ïò
	double AxialForce = 0;
	~Element_Beam();
	Element_Beam();
	Element_Beam(int id, int idNode1, int idNode2, int idsection);
};

