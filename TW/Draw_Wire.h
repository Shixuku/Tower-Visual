#pragma once
#include <vector>
#include "Node.h"
#include "Element_Wire.h"
#include <iostream>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGlyph3DMapper.h>
#include <vtkSphereSource.h>
#include <vtkNamedColors.h>
#include <vtkAutoInit.h> 

using namespace std;

class Draw_Wire
{
public:

	void ShowNodes(vector<Node>& node)const
	{
		for (auto& i : node)
		{
			i.show();
		}
	}
	void ShowElements(vector<Element_Wire>& m_Elements)const
	{
		for (auto& i : m_Elements)
		{
			i.show();
		}
	}
	void Show_Wire(vector<Node>& node, vector<Element_Wire>& m_Elements, vtkRenderer* renderer );
	
};

