#pragma once
#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractGeometry.h>
#include <vtkIdFilter.h>
#include <vtkIdTypeArray.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkCellData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVersion.h>
#include <vtkNamedColors.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkSphereSource.h>
#include <vtkAutoInit.h>
#include <vtkProperty.h>
#include<vtkLineSource.h>
#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif
class InterFace;
class InteractorStyle : public vtkInteractorStyleRubberBandPick
{
public:
	static InteractorStyle* New();
	vtkTypeMacro(InteractorStyle, vtkInteractorStyleRubberBandPick);

	InteractorStyle();//¹¹Ôìº¯Êý

	virtual void OnLeftButtonUp() override;
	void SetPoints(vtkSmartPointer<vtkPolyData> points);
	InterFace* m_pInterFace = nullptr;
	vtkIdTypeArray* ids = nullptr;
private:
	vtkSmartPointer<vtkPolyData> Points;
	vtkSmartPointer<vtkActor> SelectedActor;
	vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
};



