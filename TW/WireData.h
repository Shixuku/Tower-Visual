#pragma once
#include"Instance.h"
#include"Node.h"
class WireData:public Instance
{
public:
	int fenlie;
	double unitMass;//��λ����
	double area;//�����
	int N; //���߷ֶ���
	double rou;//bizai
	double stress;//yingli
	double strainL;
	double sag;
	vector<Node>WireSus;

};

