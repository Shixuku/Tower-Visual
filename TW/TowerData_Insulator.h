#pragma once
#include "Part_Base.h"

class TowerData_Insulator :public Part_Base
{
public:
	int m_splits;//���߷�����--����������/�߳�ȷ��//4-450
	double m_H;//��Ե�Ӹ߶�
	Node* m_node1;//��1
	Node* m_node2;//��2
	double Get_y(double x);
};

