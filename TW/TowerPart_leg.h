#pragma once
#include<vector>
#include<iostream>
using namespace std;
#include"TowerData_leg.h"

class TowerPart_leg :public TowerData_leg
{

public:
	TowerPart_leg();
	~TowerPart_leg();
	void Get_Node4(int iQuadrant, Node* n);//�õ�ֱ�������ݸ�����4����
	void G_Get_Node4(Node* n);//�õ������4����

	void Type1(int iQuadrant); //�����²�����1
	void Type2(int iQuadrant); //�����²�����2
	void Type3(int iQuadrant); //�����²�����3
	void G_Type1();//��������1
	void G_Type2();//��������2
	void G_Type3();//��������3
	void G_Type4();//��������4
	void G_Type5();//��������5
	void Create_Mesh();//ѡ�������²�����
	virtual enum Part_Type My_PartType()const
	{
		return ET_PartLeg;
	}

	QString m_Name;

};

