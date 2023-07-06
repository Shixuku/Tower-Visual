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

	//梁的截面属性
	double B_Iy=0;
	double B_Iz = 0;
	double B_Iyz = 0;
	double B_J = 0;
	//杆的截面属性
	double S = 0;//面积
	double R = 0;//直径
	int ClassSe = 0;//0代表L型，1代表圆环型
	double a = 0;
	double b = 0;
	int ClassM = 0;//代表使用哪种材料的编号
	Section(double ia, double ib, int id, int iClassSe, int iClassM);
	~Section();
	void GetLIxyz();
	void GetOIxyz();
	virtual enum Part_Type My_PartType()const;

};

