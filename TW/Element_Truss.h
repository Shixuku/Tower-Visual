#pragma once
#include "Element.h"
class Element_Truss :public Element
{
public:

	~Element_Truss();
	Element_Truss();
	Element_Truss(int id, int idNode1, int idNode2, int idsection);

};

