#pragma once
#include"Base.h"
#include<QString>
class Material :public Base
{
public:
	QString m_Name;
	double E;//����ģ��
	double Poisson;//����
	double Density;//�ܶ�
	double Thermal;//������
	Material(int id, QString iName, double iE, double iPoisson, double iDensity, double iThermal);
	~Material();
};

