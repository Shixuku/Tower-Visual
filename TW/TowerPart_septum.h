#pragma once
#include"TowerData_Body.h"

class TowerPart_septum:public TowerData_Body
{
public:
	
	void Get_Node4(int Tier, Node* n);//得到直接由数据给出的4个点
	void Type1(int Tier); //隔面类型1
	void Type2(int Tier); //隔面类型2
	void Type3(int Tier); //隔面类型3
	void Type4(int Tier); //隔面类型4
	void Type5(int Tier); //隔面类型5

	void Create_Mesh();//选择隔面下部类型 

	void SaveTo(QDataStream& fin)const
	{

	}

	void Input(QDataStream& fin)
	{

	}

	virtual enum Part_Type My_PartType()const
	{
		return ET_PartSeptum;
	}
};

