#include "Element.h"

Element::Element() 
{

}

Element::Element(int id, int idNode1, int idNode2)
{
	m_idElement = id;
	m_idNode[0] = idNode1;
	m_idNode[1] = idNode2;
}

void Element::show() const
{
	std::cout << m_idElement << "  , " << m_idNode[0] << " ,  " << m_idNode[1] << "\n";
}

void Element::showid() const
{
	std::cout << m_idElement << ", " << "\n";
}

void Element::SaveTo(QDataStream& fin) const
{
	fin << m_idElement;

	//int nNode = m_idNode.size();
	for (int i = 0; i < 2; ++i)
	{
		fin << m_idNode[i];
	}
	fin << sectionID;
	fin << ClassSectionID;
	fin << MaterialID;
	fin << groupTowerId;
}

void Element::Input(QDataStream& fin)
{
	fin >> m_idElement;

	//int nNode = m_idNode.size();
	for (int i = 0; i < 2; ++i)
	{
		fin >> m_idNode[i];
	}
	fin >> sectionID;
	fin >> ClassSectionID;
	fin >> MaterialID;
	fin >> groupTowerId;
}

