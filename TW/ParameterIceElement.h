#pragma once
#include"Base.h"
class ParameterIceElement:public Base
{
public:
	int m_id;//���
	int m_StartTime;//��ʼʱ��
	int m_EndTime;//����ʱ��
	double m_Thickness;//�������
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

