#pragma once
#include "Part_Base.h"

class TowerData_Insulator :public Part_Base
{
public:
	int m_splits;//���߷�����
	int m_type;//��Ե������
	double m_H;//��Ե�Ӹ߶�
	Node* m_node = nullptr;//������
	Node* m_node1 = nullptr;//V�͵�1
	Node* m_node2 = nullptr;//V�͵�2

	double m_W1;//��������߿�
	double m_W2;//���۶˿�
	double m_L2;//���۶˳�

	double Get_y(double x);
	double Get_y_I(double x);

};

