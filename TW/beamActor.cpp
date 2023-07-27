#include "beamActor.h"
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>


void beamActor::Calculate_Property()
{
	//利用截面形心调整坐标
	double d1 = sqrt(0.6);
	double d2 = 0;
	double w1 = 5.0 / 9;
	double w2 = 8.0 / 9;
	double ti[3], wi[3];
	ti[0] = -d1;	wi[0] = w1;
	ti[1] = d2;		wi[1] = w2;
	ti[2] = d1;		wi[2] = w1;
	double x1, y1, x2, y2;
	auto fun_x = [&](double t)->double
	{
		double x = ((1 - t) * x1 + (1 + t) * x2) / 2;
		return x;
	};
	auto fun_xy = [&](double t)->double
	{
		double x = ((1 - t) * x1 + (1 + t) * x2) / 2;
		double y = ((1 - t) * y1 + (1 + t) * y2) / 2;
		return x * y;
	};
	auto fun_x2 = [&](double t)->double
	{
		double x = ((1 - t) * x1 + (1 + t) * x2) / 2;
		return x * x;
	};

	double A = 0;
	double Sx = 0, Sy = 0;
	auto np = m_xi.size();
	for (int i = 0; i < np; ++i)
	{
		x1 = m_xi[i], y1 = m_yi[i];
		int i2 = (i + 1 == np ? 0 : i + 1);
		x2 = m_xi[i2], y2 = m_yi[i2];
		for (int j = 0; j < 3; ++j)
		{//数值积分
			double t = ti[j], w = wi[j];
			double dy = (y2 - y1) / 2;
			A += fun_x(t) * dy * w;
			Sx += fun_xy(t) * dy * w;
			Sy += fun_x2(t) * dy * w;
		}
	}
	double y0 = Sx / A;
	double x0 = Sy / A;
	for (int i = 0; i < np; ++i)
	{
		m_xi[i] -= x0;
		m_yi[i] -= y0;
	}
}

void beamActor::setNode(double x1, double y1, double z1, double x2, double y2, double z2)
{
	m_pt1[0] = x1; m_pt1[1] = y1; m_pt1[2] = z1;
	m_pt2[0] = x2; m_pt2[1] = y2; m_pt2[2] = z2;
}

void beamActor::SetSection(const double* xi, const double* yi)
{
	//设置截面环线上的各点
	m_xi.resize(6);
	m_yi.resize(6);
	for (int i = 0; i < 6; ++i)
	{
		m_xi[i] = xi[i];
		m_yi[i] = yi[i];
	}
	Calculate_Property();//利用截面形心调整坐标
}

void beamActor::Set_zAxis(double dx, double dy, double dz)
{//设置近似的z轴方向
	double Vx[3];
	Vx[0] = m_pt2[0] - m_pt1[0];
	Vx[1] = m_pt2[1] - m_pt1[1];
	Vx[2] = m_pt2[2] - m_pt1[2];
	m_L = sqrt(Vx[0] * Vx[0] + Vx[1] * Vx[1] + Vx[2] * Vx[2]);
	m_xAxis[0] = Vx[0] / m_L;
	m_xAxis[1] = Vx[1] / m_L;
	m_xAxis[2] = Vx[2] / m_L;

	m_zAxis[0] = dx;
	m_zAxis[1] = dy;
	m_zAxis[2] = dz;
	vtkMath::Cross(m_zAxis, m_xAxis, m_yAxis);
	vtkMath::Normalize(m_yAxis);
	vtkMath::Cross(m_xAxis, m_yAxis, m_zAxis);
	vtkMath::Normalize(m_zAxis);

}

void beamActor::Create_Actor(double r, double g, double b, vtkSmartPointer<vtkAppendPolyData> appendFilter) const
{
	auto np = m_xi.size();
	vtkNew<vtkPoints> points;
	points->SetNumberOfPoints(np);
	for (int i = 0; i < np; ++i)
	{
		points->SetPoint(i, m_xi[i], m_yi[i], 0);
	}
	vtkNew<vtkCellArray> poly;
	poly->InsertNextCell(np);
	for (int i = 0; i < np; ++i)
	{
		poly->InsertCellPoint(i);
	}
	vtkNew<vtkPolyData> profile;
	profile->SetPoints(points);
	profile->SetPolys(poly);


	double M[16];
	M[0] = m_zAxis[0];		M[1] = m_yAxis[0];		M[2] = -m_xAxis[0];		M[3] = m_pt1[0];
	M[4] = m_zAxis[1];		M[5] = m_yAxis[1];		M[6] = -m_xAxis[1];		M[7] = m_pt1[1];
	M[8] = m_zAxis[2];		M[9] = m_yAxis[2];		M[10] = -m_xAxis[2];	M[11] = m_pt1[2];
	M[12] = 0;				M[13] = 0;				M[14] = 0;				M[15] = 1;
	vtkNew<vtkTransform> trans;
	trans->SetMatrix(M);

	//旋转变换的Filter
	vtkNew<vtkTransformFilter> filter_trans;
	filter_trans->SetTransform(trans);
	filter_trans->SetInputData(profile);//设置旋转的面

	//定义拉伸
	vtkSmartPointer<vtkLinearExtrusionFilter> extrude = vtkSmartPointer<vtkLinearExtrusionFilter>::New();
	extrude->SetInputConnection(filter_trans->GetOutputPort());//对旋转后的截面进行拉伸
	extrude->SetExtrusionTypeToNormalExtrusion();
	extrude->SetVector(m_xAxis[0], m_xAxis[1], m_xAxis[2]);//拉伸方向
	extrude->SetScaleFactor(m_L);//拉伸长度

	appendFilter->AddInputConnection(extrude->GetOutputPort());
	appendFilter->Update();
	/*vtkNew<vtkPolyDataMapper> mapper;
	mapper->ScalarVisibilityOff();
	mapper->SetInputConnection(extrude->GetOutputPort());

	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(r, g, b);*/
}
