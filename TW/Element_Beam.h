#pragma once
#include "Element.h"
class Element_Beam :public Element
{
public:
	double direction[3] = {0,0,0};//Áºn1·½Ïò
	~Element_Beam();
	Element_Beam();
	Element_Beam(int id, int idNode1, int idNode2, int idsection,double iDirection[3]);
	Element_Beam(int id, int idNode1, int idNode2, int idsection,double iDirection[3],int a);
	void SaveTo(QDataStream& fin) const override;
	void Input(QDataStream& fin) override;
};

