#pragma once
class ParameterGalloping
{
public:
	int m_id;//�������--����̳�base�Ͳ���Ҫm_id;
	int m_AnalysisStep;//������
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

