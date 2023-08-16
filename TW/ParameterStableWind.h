#pragma once
#include<qstring.h>
class ParameterStableWind
{
public:
	int m_id;//�������--����̳�base�Ͳ���Ҫm_id;
	int m_AnalysisStep;//������
	int m_angle;
	double m_v;
	QString m_fileName;//�ļ���
	QString m_alf_select; //��ò
	int m_splitCount;
	double m_coeff; //�����ϵ��
	
	ParameterStableWind(int id, int AnalysisStep, int angle, double v, int splitCount, QString alf_select,  double coeff)
	{
		m_id = id;
		m_AnalysisStep = AnalysisStep;
		m_angle = angle;
		m_v = v;
		m_alf_select = alf_select;
		m_splitCount = splitCount;
		m_coeff = coeff;
	}
	ParameterStableWind(int id, int AnalysisStep, int angle, QString fileName, int splitCount, QString alf_select, double coeff)
	{
		m_id = id;
		m_AnalysisStep = AnalysisStep;
		m_angle = angle;
		m_fileName = fileName;
		m_alf_select = alf_select;
		m_splitCount = splitCount;
		m_coeff = coeff;
	}
};

