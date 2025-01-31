#pragma once
#include <vector>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkAppendPolyData.h>
class beamActor
{
public:
	double m_pt1[3] = {0,0,0};//节点1坐标
	double m_pt2[3]={ 0,0,0 };//节点2坐标
	double m_L=0;//单元长度
	std::vector<double> m_xi, m_yi; //截面环线上的各点
	double m_xAxis[3]={ 0,0,0 };//x轴方向
	double m_yAxis[3]= { 0,0,0 };//y轴方向
	double m_zAxis[3]= { 0,0,0 };//z轴方向
	void Calculate_Property();//利用截面形心调整坐标

	void setNode(double x1, double y1, double z1, double x2, double y2, double z2);
	void SetSection(const double* xi, const double* yi);//设置截面环线上的各点
	void Set_zAxis(double dx, double dy, double dz);//设置近似的z轴方向

	void beamActor::Create_Actor(double r, double g, double b, vtkSmartPointer<vtkAppendPolyData> appendFilter) const;
};
