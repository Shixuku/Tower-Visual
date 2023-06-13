#pragma once
#include<vtkInteractorStyleRubberBandPick.h>
#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkProperty.h>

class InterFace;

class AreaSelected_InteractorStyle : public vtkInteractorStyleRubberBandPick
{
public:
    static AreaSelected_InteractorStyle* New();
    vtkTypeMacro(AreaSelected_InteractorStyle, vtkInteractorStyleRubberBandPick);

    AreaSelected_InteractorStyle()
    {
        LastPickedActor = NULL;
        LastPickedProperty = vtkProperty::New();
        this->StartSelect();
    } 
    ~AreaSelected_InteractorStyle()
    {
        LastPickedProperty->Delete();
    }

    //void Get_PickedNode(vtkSmartPointer<vtkAreaPicker>& AreaPicker, bool bAppend);
    void Get_PickedNode(vtkSmartPointer<vtkPointPicker>& Picker, bool bAppend);

    virtual void OnLeftButtonDown() override;
    virtual void OnLeftButtonUp() override;

    InterFace* m_pInterFace = nullptr;

private:
    vtkActor* LastPickedActor;
    vtkProperty* LastPickedProperty;
};



