#pragma once
#include<qstring.h>
class ParameterStableWind
{
public:
	int m_id;//重力编号--如果继承base就不能要m_id;
	int m_AnalysisStep;//分析步
	int m_angle;
	double m_v;
	QString m_fileName;//文件名
	QString m_alf_select; //地貌
	int m_splitCount;
	double m_coeff; //风荷载系数
	
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

