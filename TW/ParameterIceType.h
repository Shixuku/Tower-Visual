#pragma once
#include"Part_Base.h"
class ParameterIceType:public Part_Base
{
public:
	int m_StartAnalysisStep;//开始时间
	int m_EndAnalysisStep;//结束时间
	double m_Thickness;//覆冰厚度
	ParameterIceType(int id, int StartAnalysisStep, int EndAnalysisStep, double Thickness)
	{
		m_id = id;
		m_StartAnalysisStep = StartAnalysisStep;
		m_EndAnalysisStep = EndAnalysisStep;
		m_Thickness = Thickness;
	}
};

