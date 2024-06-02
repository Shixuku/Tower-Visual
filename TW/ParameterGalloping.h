#pragma once
class ParameterGalloping
{
public:
	int m_id;//重力编号--如果继承base就不能要m_id;
	int m_AnalysisStep;//分析步
	double m_v;
	double m_ice;

	ParameterGalloping(int id, int AnalysisStep,  double v, double ice )
	{
		m_id = id;
		m_AnalysisStep = AnalysisStep;
		m_v = v;
		m_ice = ice;
	}


};

