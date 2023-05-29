#include "Element_Beam.h"

Element_Beam::~Element_Beam()
{
}

Element_Beam::Element_Beam()
{
}

Element_Beam::Element_Beam(int id, int idNode1, int idNode2, int idsection)
{
	m_idElement = id;
	m_idNode[0] = idNode1;
	m_idNode[1] = idNode2;
	sectionID = idsection;
}
