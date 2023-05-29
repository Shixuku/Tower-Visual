#pragma once
#include"TowerData_CrossArm.h"
class TowerPart_CrossArmSeptum :public TowerData_CrossArm
{
public:
	
	void Get_Node4(int type, Node* n, int iQuadrant, int layer);//得到直接由数据给出的4个点
	void Create_point(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n);
	void Type1(int type, int iQuadrant, int layer); //隔面类型1
	void Type2(int type, int iQuadrant, int layer); //隔面类型2
	void Type3(int type, int iQuadrant, int layer); //隔面类型3
	void Type4(int type, int iQuadrant, int layer); //隔面类型4
	void Type5(int type, int iQuadrant, int layer); //隔面类型5

	void Create_Mesh();//选择类型 
};

