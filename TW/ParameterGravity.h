#pragma once
class ParameterGravity
{
public:
	int m_id;//重力编号--如果继承base就不能要m_id;
	int m_AnalysisStep;//分析步
	int m_Direction;//方向
	double m_g;//大小
	ParameterGravity(int id, int AnalysisStep,int direction, double g)
	{
		m_id = id;
		m_AnalysisStep = AnalysisStep;
		m_Direction = direction;
		m_g = g;
	}
};

