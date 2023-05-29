#include "Section.h"

Section::Section(double ia, double ib, int id, int iClassSe, int iClassM)
{
	//Name = iName;
	ClassSe = iClassSe;
	a = ia;
	b = ib;
	ClassM = iClassM;
	m_id = id;
	if (iClassSe == 0)
	{
		GetLIxyz();
	}
	else
	{
		GetOIxyz();

	}
}

Section::~Section()
{
}

void Section::GetLIxyz()
{
	double a2 = a * a;
	double a3 = a2 * a;
	double b2 = b * b;
	double b3 = b2 * b;
	B_Iy = 1. / 12 * b * (2 * a2 - 2 * a * b + b2) * (2 * a - b);
	B_Iz = b * (2 * a3 * a - 4 * a3 * b + 8 * a2 * b2 - 6 * a * b3 + b3 * b) / (24 * a - 12 * b);
	B_Iyz = 0;
	S = 2 * a * b - b * b;
	double R2 = S / PI;
	R = sqrt(R2);
}

void Section::GetOIxyz()
{
	double D1 = 2 * a;
	double D4 = D1 * D1 * D1 * D1;
	double d = 2 * b;
	double d4 = d * d * d * d;
	B_Iy = PI / 64 * (D4 - d4);
	B_Iz = PI / 64 * (D4 - d4);
	B_Iyz = 0;
	S = PI * a * a - PI * b * b;
	double R2 = S / PI;
	R = sqrt(R2);
}

Part_Type Section::My_PartType() const
{
	return ET_Section;
}

int Section::Get_id() const
{
	return m_id;
}
