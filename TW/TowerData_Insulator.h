#pragma once
#include "Part_Base.h"

class TowerData_Insulator :public Part_Base
{
public:
	int m_splits;//���߷�����
	int m_type;//��Ե������
	double m_H;//��Ե�Ӹ߶�
	Node* m_node;//������
	Node* m_node1;//V�͵�1
	Node* m_node2;//V�͵�2
	double Get_y(double x);
};

