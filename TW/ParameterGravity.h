#pragma once
class ParameterGravity
{
public:
	int m_id;//重力编号
	int m_Direction;//方向
	double m_g;//大小
	ParameterGravity(int id, int direction, double g)
	{
		m_id = id;
		m_Direction = direction;
		m_g = g;
	}
};

