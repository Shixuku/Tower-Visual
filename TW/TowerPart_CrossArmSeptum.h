#pragma once
#include"TowerData_CrossArm.h"
class TowerPart_CrossArmSeptum :public TowerData_CrossArm
{
public:
	
	void Get_Node4(int type, Node* n, int iQuadrant, int layer);//�õ�ֱ�������ݸ�����4����
	void Create_point(const Node& n1, const Node& n2, const Node& n3, const Node& n4, Node& n);
	void Type1(int type, int iQuadrant, int layer); //��������1
	void Type2(int type, int iQuadrant, int layer); //��������2
	void Type3(int type, int iQuadrant, int layer); //��������3
	void Type4(int type, int iQuadrant, int layer); //��������4
	void Type5(int type, int iQuadrant, int layer); //��������5

	void Create_Mesh();//ѡ������ 
};

