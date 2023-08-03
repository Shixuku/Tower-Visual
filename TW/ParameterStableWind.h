#pragma once
#include<qstring.h>
class ParameterStableWind
{
public:
	int m_id;//重力编号--如果继承base就不能要m_id;
	int m_AnalysisStep;//分析步
	QString m_a;
	double m_angle;
	double m_v;
	ParameterStableWind(int id, int AnalysisStep, QString M_a, double angle, double v)
	{
		m_id = id;
		m_AnalysisStep = AnalysisStep;
		m_a = M_a;
		m_angle = angle;
		m_v = v;
	}
};

