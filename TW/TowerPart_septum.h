#pragma once
#include"TowerData_Body.h"

class TowerPart_septum:public TowerData_Body
{
public:
	
	void Get_Node4(int Tier, Node* n);//�õ�ֱ�������ݸ�����4����
	void Type1(int Tier); //��������1
	void Type2(int Tier); //��������2
	void Type3(int Tier); //��������3
	void Type4(int Tier); //��������4
	void Type5(int Tier); //��������5

	void Create_Mesh();//ѡ������²����� 

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

