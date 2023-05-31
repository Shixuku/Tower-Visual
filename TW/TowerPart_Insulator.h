#pragma once
#include"TowerData_Insulator.h"
class TowerPart_Insulator :public TowerData_Insulator
{
public:
	void Get_Nodes4(double a,Node* n);
	void Get_split1();
	void Get_split2();
	void Get_split4();
	void Get_split6();
	void Get_split8();
	void Create_Mesh();
};

