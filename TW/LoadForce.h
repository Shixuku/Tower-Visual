#pragma once
#include"Base.h"
class LoadForce:public Base
{
public:

	int m_AnalysisStep;//分析步号
	int id_node;//节点编号
	int DirectionForce;//方向
	double Force;//大小
	LoadForce(int id, int AnalysisStep, int i_node, double i_direction, double i_force);
	virtual enum Part_Type My_PartType()const
	{
		return ET_LoadForce;
	}
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
};

