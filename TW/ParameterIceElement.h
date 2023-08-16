#pragma once
#include"Base.h"
class ParameterIceElement:public Base
{
public:
	int m_type;//����
	int m_idElement;//��Ԫ���
	int m_StartAnalysisStep;//��ʼʱ��
	int m_EndAnalysisStep;//����ʱ��
	double m_Thickness;//�������
	ParameterIceElement(int type,int id, int idElement, int StartAnalysisStep, int EndAnalysisStep, double thickness)
	{
		m_type = type;
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

