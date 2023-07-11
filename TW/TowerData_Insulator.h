#pragma once
#include "Part_Base.h"

class TowerData_Insulator :public Part_Base
{
public:
	int m_splits = 0;//导线分裂数
	int m_type = 0;//绝缘子类型
	double m_H = 0;//绝缘子高度
	Node* m_node = nullptr;//悬垂点
	Node* m_node1 = nullptr;//V型点1
	Node* m_node2 = nullptr;//V型点2

	double m_W1 = 0;//连接塔身边宽
	double m_W2 = 0;//悬臂端宽
	double m_L2 = 0;//悬臂端长

	double Get_y(double x);
	double Get_y_I(double x);

};

