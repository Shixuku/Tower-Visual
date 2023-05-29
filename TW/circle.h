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
	double startPoint[3];//��һ���������
	double endPoint[3];//�ڶ����������
	double innerRadius = 0;//�ڻ��뾶
	double outerRadius = 0;//�⻷�뾶
	int step = 10;//Բ�ϵ�ĸ���

	void ConstuctRotationMatrix(vtkMatrix4x4* transformMatrix);//ת������
	void CreateCircularSection(vtkSmartPointer<vtkActor> actor);//����Actor
	void SetNode(double x1, double y1, double z1, double x2, double y2, double z2);//���ýڵ�
	void SetSection(const double xi, const double yi);//���ý��滷���ϵĸ���
	vtkSmartPointer<vtkLinearExtrusionFilter> extrusionFilter = vtkSmartPointer<vtkLinearExtrusionFilter>::New();

};
