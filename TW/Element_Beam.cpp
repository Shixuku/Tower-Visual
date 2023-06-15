#include "Element_Beam.h"
using namespace std;
Element_Beam::~Element_Beam()
{
}

Element_Beam::Element_Beam()
{
}

Element_Beam::Element_Beam(int id, int idNode1, int idNode2, int idsection,double iDirection[3])
{
	m_idElement = id;
	m_idNode[0] = idNode1;
	m_idNode[1] = idNode2;
	sectionID = idsection;
	direction[0] = iDirection[0];
	direction[1] = iDirection[1];
	direction[2] = iDirection[2];
}
