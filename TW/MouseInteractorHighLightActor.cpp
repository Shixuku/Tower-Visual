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

		//建立选择方式
		vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
		selectionNode->SetFieldType(vtkSelectionNode::POINT);//选择点
		selectionNode->SetContentType(vtkSelectionNode::INDICES);//按序号选择
		selectionNode->SetSelectionList(ids);//设置选择的依赖数据

		//建立选择操作
		vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
		selection->AddNode(selectionNode);

		//设置筛选
		vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
		extractSelection->SetInputData(0, Picker->GetDataSet());
		extractSelection->SetInputData(1, selection);

		extractSelection->Update();//执行筛选操作

		// In selection
		vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
		selected->ShallowCopy(extractSelection->GetOutput());

		vtkSmartPointer<vtkDataSetMapper> selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		selectedMapper->SetInputData(selected);
		//auto dataArray = selectedMapper->GetInput()->GetPointData()->GetArray("Address");//取得地址数组
		//auto ptrArray = dynamic_cast<vtkIdTypeArray*> (dataArray);//转换为整数数组

		if (!bAppand)
		{
			m_pInterFace->UnSelect_Nodes();
		}

		//生成新的高亮节点
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
