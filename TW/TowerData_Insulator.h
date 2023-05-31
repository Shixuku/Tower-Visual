#pragma once
#include "Part_Base.h"

class TowerData_Insulator :public Part_Base
{
public:
	int m_splits;//导线分裂数
	int m_type;//绝缘子类型
	double m_H;//绝缘子高度
	Node* m_node;//悬垂点
	Node* m_node1;//V型点1
	Node* m_node2;//V型点2
	double Get_y(double x);
};

