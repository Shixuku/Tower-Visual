#pragma once
#include"Instance.h"
#include"Node.h"
class WireData:public Instance
{
public:
	int fenlie = 0;
	double unitMass = 0;//��λ����
	double area = 0;//�����
	int N = 0; //���߷ֶ���
	double rou = 0;//bizai
	double stress = 0;//yingli
	double strainL = 0;
	double sag = 0;
	vector<Node>WireSus;
	double WireSectionId = 0;
	int wireQty = 0;
	int Creat_Node(double x, double y, double z, double f);
};

