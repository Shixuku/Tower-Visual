#pragma once
#include<QString>
#include"Base.h"
class Section_C :public Base
{public:

	QString name;
	double Iu;
	double Iv;
	double J;
	Section_C(int id, QString iname, double iIu, double iIv, double iJ);
};

