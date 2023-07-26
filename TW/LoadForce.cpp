#include "LoadForce.h"

LoadForce::LoadForce()
{
}

LoadForce::LoadForce(int id, int i_node, double i_direction, double i_force, double i_stime, double i_etime)
{
	id_force = id;
	id_node = i_node;
	DirectionForce = i_direction;
	Force = i_force;
	StartTime = i_stime;
	EndTime = i_etime;
}

void LoadForce::SaveTo(QDataStream& fin) const
{
	fin << id_force << id_node << DirectionForce << Force << StartTime << EndTime;
}

void LoadForce::Input(QDataStream& fin)
{
	fin >> id_force >> id_node >> DirectionForce >> Force >> StartTime >> EndTime;
}
