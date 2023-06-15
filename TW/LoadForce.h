#pragma once
class LoadForce
{
public:

	int id_force;
	int id_node;
	int DirectionForce;
	double Force;
	double StartTime;
	double EndTime;
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

