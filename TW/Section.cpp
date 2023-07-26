#include "Section.h"
#include <cmath>
Section::Section(QString name, double ia, double ib, int id, int iClassSe, int iClassM)
{
	m_Name = name;
	ClassSe = iClassSe;
	a = ia;
	b = ib;
	ClassM = iClassM;
	m_id = id;
	if (iClassSe == 0)
	{
		GetLIxyz();
	}
	else if(iClassSe == 1)
	{
		GetOIxyz();

	}
}

Section::Section(double ia, double ib, int id, int iClassSe, int iClassM)
{
	ClassSe = iClassSe;
	a = ia;
	b = ib;
	ClassM = iClassM;
	m_id = id;
	if (iClassSe == 0)//L
	{
		GetLIxyz();
	}
	else if(iClassSe == 1)//‘≤ª∑
	{
		GetOIxyz();

	}
	else if (iClassSe == 2)//‘≤
	{
		GetCircleIxyz();
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
	double L2 = a, t2 = b;
	B_J = pow(t2, 0.3e1) * (0.1e1 / 0.3e1 - 0.105e0 * t2 * (0.1e1 - pow(t2, 0.4e1) * pow(L2 - t2, -0.4e1) / 0.192e3) / (L2 - t2)) * (L2 - t2) +
	L2 * pow(t2, 0.3e1) * (0.1e1 / 0.3e1 - 0.21e0 * t2 * (0.1e1 - pow(t2, 0.4e1) * pow(L2, -0.4e1) / 0.12e2) / L2) + 0.7e-1 * pow(0.4e1 * t2 - 0.2e1 * sqrt(0.2e1) * sqrt(t2 * t2), 0.4e1);
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
	B_J = (PI / 32) * (pow(a, 4) - pow(a - 2 * (a-b), 4));
	S = PI * a * a - PI * b * b;
	double R2 = S / PI;
	R = sqrt(R2);
}

void Section::GetCircleIxyz()
{
	double D1 = 2 * a;
	double D4 = D1 * D1 * D1 * D1;
	B_Iy = PI / 64 * D4;
	B_Iz = PI / 64 * D4;
	B_J = (PI * pow(a, 4)) / 4;
	B_Iyz = 0;
	S = PI * a * a * 1e-6;
}

void Section::SaveTo(QDataStream& fin) const
{
	fin << a << b << m_id << ClassSe << ClassM << S << B_Iy << B_Iz << B_J;//Ωÿ√Ê±‡∫≈
}

void Section::Input(QDataStream& fin)
{
	fin >> a >> b >> m_id >> ClassSe >> ClassM >> S >> B_Iy >> B_Iz >> B_J;//Ωÿ√Ê±‡∫≈
}



Section::Section()
{
}
