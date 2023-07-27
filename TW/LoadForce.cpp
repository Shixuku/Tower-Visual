#include "LoadForce.h"
LoadForce::LoadForce(int id, int AnalysisStep, int i_node, double i_direction, double i_force)
{
	m_id = id;
	m_AnalysisStep = AnalysisStep;
	id_node = i_node;
	DirectionForce = i_direction;
	Force = i_force;
}

void LoadForce::SaveTo(QDataStream& fin) const
{
	fin << m_id << m_AnalysisStep << id_node << DirectionForce << Force;
}

void LoadForce::Input(QDataStream& fin)
{
	fin >> m_id >> m_AnalysisStep >> id_node >> DirectionForce >> Force;
}
