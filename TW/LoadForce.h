#pragma once
#include"Base.h"
class LoadForce:public Base
{
public:
	int id_force;//集中力编号
	int id_node;//节点编号
	int DirectionForce;//方向
	double Force;//大小
	double StartTime;//开始时间
	double EndTime;//结束时间
	LoadForce();
	LoadForce(int id, int i_node, double i_direction, double i_force, double i_stime, double i_etime);
	virtual enum Part_Type My_PartType()const
	{
		return ET_LoadForce;
	}
	virtual void SaveTo(QDataStream& fin)const;
	virtual void Input(QDataStream& fin);
};

