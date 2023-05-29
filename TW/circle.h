#pragma once
#include <vtkAutoInit.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkTransform.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>
#include <vtktextRenderer.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkDiskSource.h>
#include <vtkTriangle.h>
#include <vtkAutoInit.h>

class circle
{
public:
	double startPoint[3];//第一个点的坐标
	double endPoint[3];//第二个点的坐标
	double innerRadius = 0;//内环半径
	double outerRadius = 0;//外环半径
	int step = 10;//圆上点的个数

	void ConstuctRotationMatrix(vtkMatrix4x4* transformMatrix);//转化矩阵
	void CreateCircularSection(vtkSmartPointer<vtkActor> actor);//生成Actor
	void SetNode(double x1, double y1, double z1, double x2, double y2, double z2);//设置节点
	void SetSection(const double xi, const double yi);//设置截面环线上的各点
	vtkSmartPointer<vtkLinearExtrusionFilter> extrusionFilter = vtkSmartPointer<vtkLinearExtrusionFilter>::New();

};
