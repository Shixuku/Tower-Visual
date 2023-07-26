#include "InteractorStyle.h"
#include"InterFace.h"

vtkStandardNewMacro(InteractorStyle);

InteractorStyle::InteractorStyle()
{
	this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	this->SelectedActor = vtkSmartPointer<vtkActor>::New();
	this->SelectedActor->SetMapper(SelectedMapper);
}

void InteractorStyle::OnLeftButtonUp()
{
	vtkNew<vtkNamedColors> colors;

	// Forward events
	vtkInteractorStyleRubberBandPick::OnLeftButtonUp();//继承

	vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

	vtkNew<vtkExtractPolyDataGeometry> extractGeometry;
	extractGeometry->SetImplicitFunction(frustum);//传的框选里面所有东西
	extractGeometry->SetInputData(this->Points);//需要的内容 
	extractGeometry->Update();//计算 求交集

	vtkPolyData* selected = extractGeometry->GetOutput();//输出

	this->SelectedMapper->SetInputData(extractGeometry->GetOutput());
	this->SelectedMapper->ScalarVisibilityOff();//标量不显示，eg 应力颜色不显示

	ids = dynamic_cast<vtkIdTypeArray*>(selected->GetCellData()->GetArray("OriginalIds"));//动态转换
	//for (vtkIdType i = 0; i < ids->GetNumberOfTuples(); i++)
	//{
	//	//GetNumberOfTuples()//个数
	//	//ids->GetValue(i)+1就是单元编号
	//	std::cout << "Id " << i << " : " << ids->GetValue(i) << std::endl;//获取单元编号
	//}

	this->SelectedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
	this->CurrentRenderer->AddActor(SelectedActor);
	this->GetInteractor()->GetRenderWindow()->Render();
	this->HighlightProp(NULL);

	emit m_pInterFace->SignalsPickElements();
}

void InteractorStyle::SetPoints(vtkSmartPointer<vtkPolyData> points)
{
	this->Points = points;
}
