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
	Section(QString name,double ia, double ib, int id, int iClassSe, int iClassM);//����һ�� �������Ƶ�
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
	virtual void SaveTo(QDataStream& fin)const;//����
	virtual void Input(QDataStream& fin);//���

};

