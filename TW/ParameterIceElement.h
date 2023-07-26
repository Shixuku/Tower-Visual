#pragma once
#include"Base.h"
class ParameterIceElement:public Base
{
public:
	int m_id;//编号
	int m_StartTime;//开始时间
	int m_EndTime;//结束时间
	double m_Thickness;//覆冰厚度
	ParameterIceElement(int id, int start, int end, double thickness)
	{
		m_id = id;
		m_StartTime = start;
		m_EndTime = end;
		m_Thickness = thickness;
	}
	virtual enum Part_Type My_PartType()const
	{
		return ET_IceElement;
	}
};

