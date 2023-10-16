#pragma once
#include<QString>
#include"Base.h"
class Section_L:public Base
{
public:
	QString m_Name;
	double Iu;
	double Iv;
	double J;
	double A;
	Section_L(int id,QString iname, double iIu, double iIv, double iJ,double iA);
};

