#include "Draw_Wire.h"

void Draw_Wire::Show_Wire(vector<Node>& node, vector<Element_Wire>& m_Elements, vtkRenderer* renderer)
{

	vtkSmartPointer<vtkCellArray>lines = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine>line = vtkSmartPointer<vtkLine>::New();
	//点
	for (size_t i = 0; i < m_Elements.size(); i++)
	{
		line->GetPointIds()->SetId(0, m_Elements[i].m_idNode[0] - 1);
		line->GetPointIds()->SetId(1, m_Elements[i].m_idNode[1] - 1);
		lines->InsertNextCell(line);
	}
	vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
	for (int i = 0; i < node.size(); i++)
	{
		vector<double> p;
		p.resize(4);
		p[0] = node[i].m_idNode;//m_Nodes[0]指 编号为1的点
		p[1] = node[i].x;
		p[2] = node[i].y;
		p[3] = node[i].z;
		pts->InsertNextPoint(p[1], p[2], p[3]);
		Node* pNode = &node[i];	
	}

	vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
	linesPolyData->SetPoints(pts);
	linesPolyData->SetLines(lines);
	//点
	vtkSmartPointer<vtkActor> Node_actor;
	vtkSmartPointer<vtkPolyDataMapper> Node_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Node_actor = vtkSmartPointer<vtkActor>::New();
	Node_actor->SetMapper(Node_mapper);
	Node_actor->GetProperty()->SetColor(0, 0, 0);
	Node_actor->GetProperty()->SetPointSize(2);
	renderer->AddActor(Node_actor);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);
	vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
	lineActor->SetMapper(mapper);
	lineActor->GetProperty()->SetColor(0, 0, 0);
	renderer->AddActor(lineActor);
	renderer->ResetCamera();
	ShowNodes(node);
	ShowElements(m_Elements);
}
