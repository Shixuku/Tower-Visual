#include "test_pic_focusforce.h"
#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include<vtkLineSource.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkProperty.h>
#include"D:/VTK/Use_VTK.h"

void test_pic_focusforce::creat(vtkRenderer* renderer)
{
	x = m_node->x;
	y = m_node->y;
	z = m_node->z;
	if (m_x < 0)
	{
		test_x(-1, renderer);
	}
	if (m_x > 0)
	{
		test_x(1, renderer);
	}
	if (m_y < 0)
	{
		test_y(-1, renderer);
	}
	if (m_y > 0)
	{
		test_y(1, renderer);
	}
	if (m_z < 0)
	{
		test_z(-1, renderer);
	}
	if (m_z > 0)
	{
		test_z(1, renderer);
	}
}

void test_pic_focusforce::test_x(int a, vtkRenderer* renderer)
{
	// 创建锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	double x1 = x + 2000 * a;
	cone->SetCenter(x1, y, z); // 设置中心点坐标
	cone->SetHeight(500.0);     // 设置高度
	cone->SetRadius(300.0);     // 设置半径
	cone->SetDirection(1 * a, 0, 0);//设置方向--数值与大小无关
	cone->SetResolution(10);  // 设置分辨率
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(x, y, z);
	lineSource->SetPoint2(x1 + 250 * a, y, z);
	lineSource->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cone->GetOutputPort());
	mapper1->SetInputConnection(lineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor1->SetMapper(mapper1);

	renderer->AddActor(actor);
	renderer->AddActor(actor1);

	// 将渲染属性设置为线框模式
	vtkSmartPointer<vtkProperty> p_property = vtkSmartPointer<vtkProperty>::New();
	p_property->SetRepresentationToWireframe();
	actor->SetProperty(p_property);
	actor->GetProperty()->SetColor(255, 255, 0);
	actor1->GetProperty()->SetColor(255, 255, 0);
}

void test_pic_focusforce::test_y(int a, vtkRenderer* renderer)
{

	// 创建锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	double y1 = y + 2000 * a;
	cone->SetCenter(x, y1, z); // 设置中心点坐标
	cone->SetHeight(500.0);     // 设置高度
	cone->SetRadius(300.0);     // 设置半径
	cone->SetDirection(0, 1 * a, 0);//设置方向--数值与大小无关
	cone->SetResolution(10);  // 设置分辨率
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(x, y, z);
	lineSource->SetPoint2(x, y1 + 250 * a, z);
	lineSource->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cone->GetOutputPort());
	mapper1->SetInputConnection(lineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor1->SetMapper(mapper1);

	renderer->AddActor(actor);
	renderer->AddActor(actor1);

	// 将渲染属性设置为线框模式
	vtkSmartPointer<vtkProperty> p_property = vtkSmartPointer<vtkProperty>::New();
	p_property->SetRepresentationToWireframe();
	actor->SetProperty(p_property);
	actor->GetProperty()->SetColor(255, 255, 0);
	actor1->GetProperty()->SetColor(255, 255, 0);
}

void test_pic_focusforce::test_z(int a, vtkRenderer* renderer)
{
	// 创建锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	double z1 = z + 2000 * a;
	cone->SetCenter(x, y, z1); // 设置中心点坐标
	cone->SetHeight(500.0);     // 设置高度
	cone->SetRadius(300.0);     // 设置半径
	cone->SetDirection(0, 0, 1 * a);//设置方向--数值与大小无关
	cone->SetResolution(10);  // 设置分辨率
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(x, y, z);
	lineSource->SetPoint2(x, y, z1 + 250 * a);
	lineSource->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cone->GetOutputPort());
	mapper1->SetInputConnection(lineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor1->SetMapper(mapper1);

	renderer->AddActor(actor);
	renderer->AddActor(actor1);

	// 将渲染属性设置为线框模式
	vtkSmartPointer<vtkProperty> p_property = vtkSmartPointer<vtkProperty>::New();
	p_property->SetRepresentationToWireframe();
	actor->SetProperty(p_property);
	actor->GetProperty()->SetColor(255, 255, 0);
	actor1->GetProperty()->SetColor(255, 255, 0);
}
