#pragma once
#include"Part_Base.h"
class ParameterIceType:public Part_Base
{
public:
	int m_StartAnalysisStep;//��ʼʱ��
	//int m_EndAnalysisStep;//����ʱ��
	double m_Thickness;//�������
	QString m_WireType;//��������
	ParameterIceType(int id, int StartAnalysisStep,double Thickness, QString WireType)
	{
		m_id = id;
		m_StartAnalysisStep = StartAnalysisStep;
		//m_EndAnalysisStep = EndAnalysisStep;
		m_Thickness = Thickness;
		m_WireType = WireType;
	}
};

