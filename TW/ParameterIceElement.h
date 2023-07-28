#pragma once
#include"Base.h"
class ParameterIceElement:public Base
{
public:
	int m_idElement;//单元编号
	int m_StartAnalysisStep;//开始时间
	int m_EndAnalysisStep;//结束时间
	double m_Thickness;//覆冰厚度
	ParameterIceElement(int id, int idElement, int StartAnalysisStep, int EndAnalysisStep, double thickness)
	{
		m_id = id;
		m_idElement = idElement;
		m_StartAnalysisStep = StartAnalysisStep;
		m_EndAnalysisStep = EndAnalysisStep;
		m_Thickness = thickness;
	}
	virtual enum Part_Type My_PartType()const
	{
		return ET_IceElement;
	}
};

