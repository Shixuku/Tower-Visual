#include "AreaSelected_InteractorStyle.h"
#include"InterFace.h"
#include "vtkIdTypeArray.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkExtractSelection.h"
#include "vtkDataSetMapper.h"
#include "vtkPoints.h"
#include "vtkExtractSelectedIds.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkPicker.h"
#include "vtkSelectVisiblePoints.h"
#include <vtkNamedColors.h>


vtkStandardNewMacro(AreaSelected_InteractorStyle);


void AreaSelected_InteractorStyle::OnLeftButtonDown()
{
    vtkNew<vtkNamedColors> colors;
    int* clickPos = this->GetInteractor()->GetEventPosition();
    // Pick from this location.
    vtkNew<vtkPointPicker> picker;
    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

    vtkInteractorStyleRubberBandPick::OnLeftButtonDown(); // 调用基类的方法以完成默认的框选操作
}

void AreaSelected_InteractorStyle::OnLeftButtonUp()
{
    vtkInteractorStyleRubberBandPick::OnLeftButtonUp(); // 调用基类的方法以完成默认的框选操作

    vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
    int* startPoint = this->StartPosition;
    int* endPoint = this->EndPosition;
   
     // 创建点拾取器
    vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();
    pointPicker->SetTolerance(1);
    vtkSmartPointer<vtkIdTypeArray> pointIds = vtkSmartPointer<vtkIdTypeArray>::New();
    pointIds->SetNumberOfComponents(1);
    // 遍历区域内的点进行拾取
    double w = abs(endPoint[0] - startPoint[0]);
    double h = abs(endPoint[1] - startPoint[1]);
    if (w < 0.001)return;

    if (startPoint[0] > endPoint[0])swap(startPoint[0], endPoint[0]);
    if (startPoint[1] > endPoint[1])swap(startPoint[1], endPoint[1]);
    int times = 1;
    // 存储已选取的点的ID
    std::set<vtkIdType> selectedPointIds;
    for (double x = startPoint[0]; x <= endPoint[0]; x += w / 10) {
        times++;
        if (times > 10)break;
        for (double y = startPoint[1]; y <= endPoint[1]; y += h / 10) {
            pointPicker->Pick(x, y, 0, this->GetDefaultRenderer());

            // 检查点是否已经被选取过
            vtkIdType pointId = pointPicker->GetPointId();
            if (selectedPointIds.find(pointId) != selectedPointIds.end()) {
                continue; // 跳过已选取的点
            }
            // 将点的ID添加到选取集合中
            selectedPointIds.insert(pointId);

            Get_PickedNode(pointPicker, this->GetInteractor()->GetShiftKey() > 0);
        }
    }

}

void AreaSelected_InteractorStyle::Get_PickedNode(vtkSmartPointer<vtkPointPicker>& Picker, bool bAppend)
{
    if (Picker->GetPointId() < 0) //
    {
        m_pInterFace->UnSelect_Nodes();
    }
    else if (Picker->GetPointId() >= 0)
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

        if (!bAppend)
        {
            m_pInterFace->UnSelect_Nodes();
        }

        //生成新的高亮节点
        vtkSmartPointer<vtkActor> ActorSelected = vtkSmartPointer<vtkActor>::New();
        ActorSelected->SetMapper(selectedMapper);
        ActorSelected->GetProperty()->SetColor(1, 0, 0);
        ActorSelected->GetProperty()->SetPointSize(10);
        ActorSelected->PickableOff();
        m_pInterFace->m_Renderer_2->AddActor(ActorSelected);
        m_pInterFace->m_Renderer_2->Render();
        m_pInterFace->m_Renderer->AddActor(ActorSelected);
        m_pInterFace->m_Renderer->Render();
        //		m_pInterFace->m_NodeSelected.push_back(ActorSelected);
        m_pInterFace->Add_Select(ActorSelected);

        emit m_pInterFace->Msg_Select_Nodes();
    }
}