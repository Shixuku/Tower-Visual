#pragma once
#include<string>
#include <numbers>
#include"Base.h"
# define PI acos(-1)
using namespace std;
class Section: public Base
{
public:
	string Name;
	Section();

	//���Ľ�������
	double B_Iy=0;
	double B_Iz = 0;
	double B_Iyz = 0;
	double B_J = 0;
	//�˵Ľ�������
	double S = 0;//���
	double R = 0;//ֱ��
	int ClassSe = 0;//0����L�ͣ�1����Բ����
	double a = 0;
	double b = 0;
	int ClassM = 0;//����ʹ�����ֲ��ϵı��
	Section(double ia, double ib, int id, int iClassSe, int iClassM);
	~Section();
	void GetLIxyz();
	void GetOIxyz();
	virtual enum Part_Type My_PartType()const;

};

