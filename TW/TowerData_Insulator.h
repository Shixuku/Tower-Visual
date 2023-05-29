#pragma once
#include "Part_Base.h"
class TowerData_Insulator :public Part_Base
{
public:
	int m_splits;//导线分裂数
	int m_type;//类型
	double m_H;//绝缘子高度
};

