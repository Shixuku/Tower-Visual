#include "Node.h"

Node::Node(int id, double ix, double iy, double iz, double iF)
{
	m_idNode = id, x = ix, y = iy, z = iz, F = iF;
}

Node::Node(int id, double ix, double iy, double iz, double iF, int iType)
{
	m_idNode = id, x = ix, y = iy, z = iz, F = iF, Type = iType;
}

void Node::show() const
{
	std::cout << m_idNode << ", " << x << " ," << y << " ," << z << "  " << F << "\n";
}

void Node::SaveTo(QDataStream& fin) const
{
	fin << m_idNode << x << y << z << restraint << groupTowerId;
}

void Node::Input(QDataStream& fin)
{
	fin >> m_idNode >> x >> y >> z >> restraint >> groupTowerId;
}
