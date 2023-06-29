#pragma once
class LoadForce
{
public:
	int id_force;//集中力编号
	int id_node;//节点编号
	int DirectionForce;//方向
	double Force;//大小
	double StartTime;//开始时间
	double EndTime;//结束时间
	LoadForce(int id, int i_node,double i_direction, double i_force, double i_stime,double i_etime)
	{
		id_force = id;
		id_node = i_node;
		DirectionForce = i_direction;
		Force = i_force;
		StartTime = i_stime;
		EndTime = i_etime;
	}
};

