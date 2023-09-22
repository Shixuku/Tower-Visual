#pragma once
#include<string>
#include <numbers>
#include"Base.h"
# define PI acos(-1)
using namespace std;
class Section: public Base
{
public:
	QString m_Name;
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
	Section(QString name,double ia, double ib, int id, int iClassSe, int iClassM);//重载一个 加了名称的
	Section(double ia, double ib, int id, int iClassSe, int iClassM);
	Section(int id, QString name, double iIy, double iTz, int iJ, int iClassM);
	~Section();
	void GetLIxyz();
	void GetOIxyz();
	void GetCircleIxyz();
	virtual enum Part_Type My_PartType()const
	{
		return ET_Section;
	}
	virtual void SaveTo(QDataStream& fin)const;//保存
	virtual void Input(QDataStream& fin);//输出

};

