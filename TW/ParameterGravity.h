#pragma once
class ParameterGravity
{
public:
	int m_id;//�������--����̳�base�Ͳ���Ҫm_id;
	int m_AnalysisStep;//������
	int m_Direction;//����
	double m_g;//��С
	ParameterGravity(int id, int AnalysisStep,int direction, double g)
	{
		m_id = id;
		m_AnalysisStep = AnalysisStep;
		m_Direction = direction;
		m_g = g;
	}
};

