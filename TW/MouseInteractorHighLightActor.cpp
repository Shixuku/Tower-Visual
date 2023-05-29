#include "MouseInteractorHighLightActor.h"
#include"InterFace.h"
#include <vtkNamedColors.h>
#include <vtkIdTypeArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkPointData.h>

vtkStandardNewMacro(MouseInteractorHighLightActor);

void MouseInteractorHighLightActor::OnLeftButtonDown()
{
	vtkNew<vtkNamedColors> colors;

	int* clickPos = this->GetInteractor()->GetEventPosition();

	// Pick from this location.
	vtkNew<vtkPointPicker> picker;
	picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

	Get_PickedNode(picker, this->GetInteractor()->GetShiftKey() > 0);

	// Forward events
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void MouseInteractorHighLightActor::Get_PickedNode(vtkPointPicker* Picker, bool bAppand)
{
	if (Picker->GetPointId() >= 0)
	{
		vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
		ids->SetNumberOfComponents(1);
		ids->InsertNextValue(Picker->GetPointId());

		//����ѡ��ʽ
		vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
		selectionNode->SetFieldType(vtkSelectionNode::POINT);//ѡ���
		selectionNode->SetContentType(vtkSelectionNode::INDICES);//�����ѡ��
		selectionNode->SetSelectionList(ids);//����ѡ�����������

		//����ѡ�����
		vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
		selection->AddNode(selectionNode);

		//����ɸѡ
		vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
		extractSelection->SetInputData(0, Picker->GetDataSet());
		extractSelection->SetInputData(1, selection);

		extractSelection->Update();//ִ��ɸѡ����

		// In selection
		vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
		selected->ShallowCopy(extractSelection->GetOutput());

		vtkSmartPointer<vtkDataSetMapper> selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		selectedMapper->SetInputData(selected);
		//auto dataArray = selectedMapper->GetInput()->GetPointData()->GetArray("Address");//ȡ�õ�ַ����
		//auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//ת��Ϊ��������

		if (!bAppand)
		{
			m_pInterFace->UnSelect_Nodes();
		}

		//�����µĸ����ڵ�
		vtkSmartPointer<vtkActor> ActorSelected = vtkSmartPointer<vtkActor>::New();
		ActorSelected->SetMapper(selectedMapper);
		ActorSelected->GetProperty()->SetColor(1, 0, 0);
		ActorSelected->GetProperty()->SetPointSize(10);
		ActorSelected->PickableOff();
		m_pInterFace->m_Renderer->AddActor(ActorSelected);
		//		m_pInterFace->m_NodeSelected.push_back(ActorSelected);
		m_pInterFace->Add_Select(ActorSelected);

		emit m_pInterFace->Msg_Select_Nodes();
	}
}
