#pragma once
#include "Element.h"
class Element_Beam :public Element
{
public:

	~Element_Beam();
	Element_Beam();
	Element_Beam(int id, int idNode1, int idNode2, int idsection);
};

