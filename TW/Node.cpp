#include "Node.h"

Node::Node(int id, double ix, double iy, double iz)
{
	m_idNode = id, x = ix, y = iy, z = iz;
}

void Node::show() const
{
	std::cout << m_idNode << ", " << x << " ," << y << " ," << z << "\n";
}

void Node::SaveTo(QDataStream& fin) const
{
	fin << m_idNode << x << y << z;
}

void Node::Input(QDataStream& fin)
{
	fin >> m_idNode >> x >> y >> z;
}
