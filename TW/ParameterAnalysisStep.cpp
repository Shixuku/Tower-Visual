#include "ParameterAnalysisStep.h"

ParameterAnalysisStep::ParameterAnalysisStep(QString Name, int id, QString Type, double TimeSlot, double TimeIncrement, double MinAllowableValue, int MaxEpoch)
{
	m_Name = Name;
	m_id = id;
	m_Type = Type;
	m_TimeSlot = TimeSlot;
	m_TimeIncrement = TimeIncrement;
	m_MinAllowableValue = MinAllowableValue;
	m_MaxEpoch = MaxEpoch;
}
