#include "Element_Truss.h"

Element_Truss::~Element_Truss()
{
}

Element_Truss::Element_Truss()
{
}

Element_Truss::Element_Truss(int id, int idNode1, int idNode2, int idsection, double axialforce)
{
	m_idElement = id;
	m_idNode[0] = idNode1;
	m_idNode[1] = idNode2;
	ClassSectionID = idsection;
	AxialForce = axialforce;
}

Element_Truss::Element_Truss(int id, int idNode1, int idNode2, int idsection, double axialforce, QString iType)
{
	m_idElement = id;
	m_idNode[0] = idNode1;
	m_idNode[1] = idNode2;
	ClassSectionID = idsection;
	AxialForce = axialforce;
	Type = iType;
}

