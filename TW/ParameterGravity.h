#pragma once
class ParameterGravity
{
public:
	int m_id;//�������
	int m_Direction;//����
	double m_g;//��С
	ParameterGravity(int id, int direction, double g)
	{
		m_id = id;
		m_Direction = direction;
		m_g = g;
	}
};

