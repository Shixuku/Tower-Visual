#pragma once
#include<QString>
#include"Base.h"
class ParameterAnalysisStep:public Base
{
public:
	//int m_id;//编号
	QString m_Name;//分析步名称
	QString m_Type;//分析步类型 静力还是动力
	double m_TimeSlot;//时间段
	double m_TimeIncrement;//时间增量
	double m_MinAllowableValue;//最小容许值
	int m_MaxEpoch;//最大迭代次数

	ParameterAnalysisStep(QString Name,int id, QString Type,double TimeSlot, double TimeIncrement, double MinAllowableValue, int MaxEpoch);

};

