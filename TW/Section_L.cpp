#include "Section_L.h"

Section_L::Section_L(int id, QString iname, double iIu, double iIv, double iJ, double iA)
{
	m_id = id;
	m_Name = iname;
	Iu = iIu;
	Iv = iIv;
	J = iJ;
	A = iA;
}
