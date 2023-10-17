#pragma once
#include"Base.h"
#include<QString>
class Material :public Base
{
public:
	QString m_Name;
	double E;//弹性模量
	double Poisson;//泊松
	double Density;//密度
	double UltimateStress;//极限应力
	double Thermal;//热膨胀
	Material(int id, QString iName, double iE, double iPoisson, double iDensity,double iStress, double iThermal);
	~Material();
};

