#pragma once
#include"Base.h"
class LoadForce:public Base
{
public:

	int m_AnalysisStep;//��������
	int id_node;//�ڵ���
	int DirectionForce;//����
	double Force;//��С
	LoadForce(int id, int AnalysisStep, int i_node, double i_direction, double i_force);
	virtual enum Part_Type My_PartType()const
	{
		return ET_LoadForce;
	}
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
};

