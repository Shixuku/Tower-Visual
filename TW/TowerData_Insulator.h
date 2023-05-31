#pragma once
#include "Part_Base.h"

class TowerData_Insulator :public Part_Base
{
public:
	int m_splits;//导线分裂数--决定几边形/边长确定//4-450
	double m_H;//绝缘子高度
	Node* m_node1;//点1
	Node* m_node2;//点2
	double Get_y(double x);
};

