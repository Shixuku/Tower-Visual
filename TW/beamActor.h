#pragma once
#include <vector>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
class beamActor
{
public:
	double m_pt1[3] = {0,0,0};//�ڵ�1����
	double m_pt2[3]={ 0,0,0 };//�ڵ�2����
	double m_L=0;//��Ԫ����
	std::vector<double> m_xi, m_yi; //���滷���ϵĸ���
	double m_xAxis[3]={ 0,0,0 };//x�᷽��
	double m_yAxis[3]= { 0,0,0 };//y�᷽��
	double m_zAxis[3]= { 0,0,0 };//z�᷽��
	void Calculate_Property();//���ý������ĵ�������

	void setNode(double x1, double y1, double z1, double x2, double y2, double z2);
	void SetSection(const double* xi, const double* yi);//���ý��滷���ϵĸ���
	void Set_zAxis(double dx, double dy, double dz);//���ý��Ƶ�z�᷽��

	void beamActor::Create_Actor(double r, double g, double b, vtkSmartPointer<vtkActor> actor) const;
};
