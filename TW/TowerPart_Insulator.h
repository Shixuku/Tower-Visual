#pragma once
#include"TowerData_Insulator.h"
#include "Part_Base.h"
class TowerPart_Insulator :public TowerData_Insulator
{
public:
	int ArmId = 0;
	int wireLogo = 0;
	void Get_Nodes4(double a, Node* n);
	void Get_Nodes4_I(double a, Node* n);
	void Get_split1();
	void Get_split1_I();
	void Get_split2();
	void Get_split2_I();
	void Get_split4(int m_type);
	void Get_split6(int m_type);
	void Get_split8(int m_type);
	void Create_Mesh();

	virtual enum Part_Type My_PartType()const
	{
		return ET_PartInsulator;
	}
};

