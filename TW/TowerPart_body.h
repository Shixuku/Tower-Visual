#pragma once
#include"TowerData_Body.h"

class TowerPart_body: public TowerData_Body
{
public:
	void Get_Node4(int iQuadrant, Node* n, int layer);//�õ�ֱ�������ݸ�����4����

	//����
	void type1(int iQuadrant, int layer);//tuiup1 iQuadrant--���� 
	void type2(int iQuadrant, int layer);//tuiup2
	void type3(int iQuadrant, int layer);//tuiup3
	void type4(int iQuadrant, int layer);//shen1
	void type5(int iQuadrant, int layer);//shen2
	void type6(int iQuadrant, int layer);//shen3
	void type7(int iQuadrant, int layer);//shen4
	void type8(int iQuadrant, int layer);//shen5
	void type9(int iQuadrant, int layer);//shen6
	void type10(int iQuadrant, int layer);//shen7
	void type11(int iQuadrant, int layer);
	void type12(int iQuadrant, int layer);
	void type13(int iQuadrant, int layer);//shen2+1

	//����
	void G_Get_Node4(int Tier, Node* n,int pos);//�õ�ֱ�������ݸ�����4���� pos=0���� pos=1�м�
	void G_Type1(int Tier,int pos); //��������1
	void G_Type2(int Tier, int pos); //��������2
	void G_Type3(int Tier, int pos); //��������3
	void G_Type4(int Tier, int pos); //��������4
	void G_Type5(int Tier, int pos); //��������5
	void G_Type6(int Tier, int pos); //��������5

	void Create_Mesh();

	virtual enum Part_Type My_PartType()const
	{
		return ET_PartBody;
	}
	//QString m_name;
};

