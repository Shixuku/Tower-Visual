#pragma once
#include<QString>
#include"Base.h"
class ParameterAnalysisStep:public Base
{
public:
	//int m_id;//���
	QString m_Name;//����������
	QString m_Type;//���������� �������Ƕ���
	double m_TimeSlot;//ʱ���
	double m_TimeIncrement;//ʱ������
	double m_MinAllowableValue;//��С����ֵ
	int m_MaxEpoch;//����������

	ParameterAnalysisStep(QString Name,int id, QString Type,double TimeSlot, double TimeIncrement, double MinAllowableValue, int MaxEpoch);

};

