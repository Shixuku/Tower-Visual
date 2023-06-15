#include <iostream>
#include <vtkLine.h>
#include <vtkTubeFilter.h>
#include"circle.h"
#include <vector>



void circle::ConstuctRotationMatrix(vtkMatrix4x4* transformMatrix)
{
	// 计算连接这两个点的向量
	double normal[3] = { endPoint[0] - startPoint[0], endPoint[1] - startPoint[1], endPoint[2] - startPoint[2] };

	// 定义两个向量
	double sourceVector[3] = { 0.0, 0.0, 1.0 };
	double targetVector[3] = { normal[0], normal[1], normal[2] };

	// 标准化两个向量
	vtkMath::Normalize(sourceVector);
	vtkMath::Normalize(targetVector);

	// 计算旋转轴和旋转角度
	// 两个向量的点积等于一个向量在另一个向量方向上的投影长度与另一个向量本身长度的乘积，反映了一个向量在另一个向量方向上的分量大小。
	// 除以两个向量的长度即可得到两个向量的夹角
	double rotationAxis[3];
	vtkMath::Cross(sourceVector, targetVector, rotationAxis);
	double rotationAngle = acos(vtkMath::Dot(sourceVector, targetVector));

	// 构建旋转矩阵
	vtkSmartPointer<vtkTransform> rotation = vtkSmartPointer<vtkTransform>::New();
	rotation->RotateWXYZ(rotationAngle * vtkMath::DegreesFromRadians(1.0), rotationAxis);

	// 构建平移矩阵
	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	// translation->Translate(startPoint);

	// 构建旋转和平移的组合矩阵
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Concatenate(translation);
	transform->Concatenate(rotation);

	// 获取变换矩阵
	transform->GetMatrix(transformMatrix);

	// 输出旋转矩阵
	// transformMatrix->Print(cout);
}

/* 创建环形截面，三角区域 */
void circle::CreateCircularSection(vtkSmartPointer<vtkActor> actor)
{
	vtkSmartPointer<vtkLinearExtrusionFilter> extrusionFilter = vtkSmartPointer<vtkLinearExtrusionFilter>::New();
	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
	vtkSmartPointer<vtkPoints> innerCirclePoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> outerCirclePoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> innerCircleCells = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkCellArray> outerCircleCells = vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkMatrix4x4> transformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	ConstuctRotationMatrix(transformMatrix);

	// Create points for inner circle
	for (double i = 0; i < 360; i += step)
	{
		double radians = vtkMath::RadiansFromDegrees(i);
		double x = innerRadius * cos(radians);
		double y = innerRadius * sin(radians);
		innerCirclePoints->InsertNextPoint(x, y, 0.0);
	}

	// Create cells
	int numberOfCell = 360 / step;

	// Create points for outer circle
	for (double i = 0; i < 360; i += step)
	{
		double radians = vtkMath::RadiansFromDegrees(i);
		double x = outerRadius * cos(radians);
		double y = outerRadius * sin(radians);
		innerCirclePoints->InsertNextPoint(x, y, 0.0);
	}

	// transform the points using the rotation matrix and the center point
	for (vtkIdType i = 0; i < numberOfCell * 2; i++)
	{
		double point[3];
		innerCirclePoints->GetPoint(i, point);
		transformMatrix->MultiplyPoint(point, point);
		point[0] += startPoint[0];
		point[1] += startPoint[1];
		point[2] += startPoint[2];
		innerCirclePoints->SetPoint(i, point);
	}

	for (int i = 0; i < numberOfCell; i++)
	{
		vtkSmartPointer<vtkTriangle> triangle1 = vtkSmartPointer<vtkTriangle>::New();
		triangle1->GetPointIds()->SetId(0, i);
		triangle1->GetPointIds()->SetId(1, (i + 1) % numberOfCell);
		triangle1->GetPointIds()->SetId(2, i + numberOfCell);
		innerCircleCells->InsertNextCell(triangle1);

		vtkSmartPointer<vtkTriangle> triangle2 = vtkSmartPointer<vtkTriangle>::New();
		triangle2->GetPointIds()->SetId(0, (i + 1) % numberOfCell);
		triangle2->GetPointIds()->SetId(1, (i + 1) % numberOfCell + numberOfCell);
		triangle2->GetPointIds()->SetId(2, i + numberOfCell);
		outerCircleCells->InsertNextCell(triangle2);
	}

	// Create polydata
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(innerCirclePoints);
	polyData->SetPolys(innerCircleCells);

	vtkSmartPointer<vtkPolyData> outerCirclePolyData = vtkSmartPointer<vtkPolyData>::New();
	outerCirclePolyData->SetPoints(innerCirclePoints);
	outerCirclePolyData->SetPolys(outerCircleCells);

	// Append outer circle to polydata
	appendFilter->AddInputData(polyData);
	appendFilter->AddInputData(outerCirclePolyData);
	appendFilter->Update();

	extrusionFilter->SetInputData(appendFilter->GetOutput());
	extrusionFilter->SetExtrusionTypeToNormalExtrusion();
	extrusionFilter->SetVector(endPoint[0] - startPoint[0], endPoint[1] - startPoint[1], endPoint[2] - startPoint[2]);
	extrusionFilter->Update();
	// Convert to triangle mesh
	vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputConnection(extrusionFilter->GetOutputPort());
	triangleFilter->Update();

	// Create mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(triangleFilter->GetOutputPort());

	actor->SetMapper(mapper);

}




//void circle::CreateActor()
//{
//	
//
//
//	extrusionFilter->SetInputData(appendFilter->GetOutput());
//	extrusionFilter->SetExtrusionTypeToNormalExtrusion();
//	extrusionFilter->SetVector(endPoint[0] - startPoint[0], endPoint[1] - startPoint[1], endPoint[2] - startPoint[2]);
//	extrusionFilter->Update();
//	// Convert to triangle mesh
//	vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
//	triangleFilter->SetInputConnection(extrusionFilter->GetOutputPort());
//	triangleFilter->Update();
//
//	// Create mapper and actor
//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	mapper->SetInputConnection(triangleFilter->GetOutputPort());
//
//	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//	actor->SetMapper(mapper);
//
//	//// Create renderer, render window, and interactor
//	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//	//renderWindow->AddRenderer(renderer);
//	//vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	//interactor->SetRenderWindow(renderWindow);
//
//	//// Add actor to renderer and set background color
//	//renderer->AddActor(actor);
//	//renderer->SetBackground(0.2, 0.3, 0.4);
//
//	//// Start interactor
//	//renderWindow->Render();
//	//interactor->Start();
//
//}

void circle::SetNode(double x1, double y1, double z1, double x2, double y2, double z2)
{
	startPoint[0] = x1; startPoint[1] = y1; startPoint[2] = z1;
	endPoint[0] = x2; endPoint[1] = y2; endPoint[2] = z2;
}

void circle::SetSection(const double xi, const double yi)
{
	outerRadius = xi;
	innerRadius = yi;
}


