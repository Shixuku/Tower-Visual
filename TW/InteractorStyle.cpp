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
	vtkInteractorStyleRubberBandPick::OnLeftButtonUp();//�̳�

	vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

	vtkNew<vtkExtractPolyDataGeometry> extractGeometry;
	extractGeometry->SetImplicitFunction(frustum);//���Ŀ�ѡ�������ж���
	extractGeometry->SetInputData(this->Points);//��Ҫ������ 
	extractGeometry->Update();//���� �󽻼�

	vtkPolyData* selected = extractGeometry->GetOutput();//���

	this->SelectedMapper->SetInputData(extractGeometry->GetOutput());
	this->SelectedMapper->ScalarVisibilityOff();//��������ʾ��eg Ӧ����ɫ����ʾ

	ids = dynamic_cast<vtkIdTypeArray*>(selected->GetCellData()->GetArray("OriginalIds"));//��̬ת��
	//for (vtkIdType i = 0; i < ids->GetNumberOfTuples(); i++)
	//{
	//	//GetNumberOfTuples()//����
	//	//ids->GetValue(i)+1���ǵ�Ԫ���
	//	std::cout << "Id " << i << " : " << ids->GetValue(i) << std::endl;//��ȡ��Ԫ���
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
