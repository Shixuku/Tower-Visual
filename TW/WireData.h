#pragma once
#include"Instance.h"
#include"Node.h"
class WireData:public Instance
{
public:
	int fenlie;
	double unitMass;//单位质量
	double area;//截面积
	int N; //导线分段数
	double rou;//bizai
	double stress;//yingli
	double strainL;
	double sag;
	vector<Node>WireSus;

};

