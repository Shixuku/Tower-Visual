#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkPointPicker.h>
class InterFace;
class MouseInteractorHighLightActor :public vtkInteractorStyleTrackballCamera
{
public:
	static MouseInteractorHighLightActor* New();
	vtkTypeMacro(MouseInteractorHighLightActor, vtkInteractorStyleTrackballCamera);

	MouseInteractorHighLightActor()
	{
		LastPickedActor = NULL;
		LastPickedProperty = vtkProperty::New();
	}
	virtual ~MouseInteractorHighLightActor()
	{
		LastPickedProperty->Delete();
	}

	virtual void OnLeftButtonDown() override;

	void Get_PickedNode(vtkPointPicker* Picker, bool bAppand);

	InterFace* m_pInterFace = nullptr;

private:
	vtkActor* LastPickedActor;
	vtkProperty* LastPickedProperty;
};

