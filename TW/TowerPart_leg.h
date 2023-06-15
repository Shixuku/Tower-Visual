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
	void Get_Node4(int iQuadrant, Node* n);//得到直接由数据给出的4个点
	void G_Get_Node4(Node* n);//得到隔面的4个点

	void Type1(int iQuadrant); //塔腿下部类型1
	void Type2(int iQuadrant); //塔腿下部类型2
	void Type3(int iQuadrant); //塔腿下部类型3
	void G_Type1();//隔面类型1
	void G_Type2();//隔面类型2
	void G_Type3();//隔面类型3
	void G_Type4();//隔面类型4
	void G_Type5();//隔面类型5
	void Create_Mesh();//选择塔腿下部类型
	virtual enum Part_Type My_PartType()const
	{
		return ET_PartLeg;
	}

	QString m_Name;

};

