#include "Constrained_image.h"
#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
void Constrained_image::Draw_fixed_Constrained(double x, double y, double z, vtkRenderer* renderer)
{
    struct ConeInfo
    {
        double radius;
        double height;
        double direction[3];
        double center[3];
        double color[3];
    };

    ConeInfo cones[] = {
        { 200.0, 600.0, { 0, 1, 0 }, { x, y - 300, z }, { 0.0, 0.0, 1.0 } },  // y方向
        { 300, 1200.0, { 0, 1, 0 }, { x, y - 600, z }, { 0.0, 0.0, 1.0 } },
        { 300, 500.0, { 0, 1, 0 }, { x, y - 250, z }, { 1.0, 0.5, 0.0 } },
        { 200.0, 600.0, { 1, 0, 0 }, { x - 300, y, z }, { 0.0, 0.0, 1.0 } },  // x方向
        { 300, 1200.0, { 1, 0, 0 }, { x - 600, y, z }, { 0.0, 0.0, 1.0 } },
        { 300, 500.0, { 1, 0, 0 }, { x - 250, y, z }, { 1.0, 0.5, 0.0 } },
        { 200.0, 600.0, { 0, 0, -1 }, { x, y, z + 300 }, { 0.0, 0.0, 1.0 } },  // z方向
        { 300, 1200.0, { 0, 0, -1 }, { x, y, z + 600 }, { 0.0, 0.0, 1.0 } },
        { 300, 500.0, { 0, 0, -1 }, { x, y, z + 250 }, { 1.0, 0.5, 0.0 } }
    };

    int numCones = sizeof(cones) / sizeof(cones[0]);

    for (int i = 0; i < numCones; i++)
    {
        vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
        coneSource->SetRadius(cones[i].radius);
        coneSource->SetHeight(cones[i].height);
        coneSource->SetDirection(cones[i].direction);
        coneSource->SetCenter(cones[i].center);
        coneSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        coneMapper->SetInputConnection(coneSource->GetOutputPort());

        vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
        coneActor->SetMapper(coneMapper);
        coneActor->GetProperty()->SetColor(cones[i].color);
        coneActor->GetProperty()->SetRepresentationToWireframe();

        renderer->AddActor(coneActor);
    }

    renderer->ResetCamera();
}

void Constrained_image::Draw_hinge_joint(double x, double y, double z, vtkRenderer* renderer)
{
    struct ConeInfo
    {
        double radius;
        double height;
        double direction[3];
        double center[3];
        double color[3];
    };

    ConeInfo cones[] = {
      
        { 300, 500.0, { 0, 1, 0 }, { x, y - 250, z }, { 1.0, 0.5, 0.0 } },
        { 300, 500.0, { 1, 0, 0 }, { x - 250, y, z }, { 1.0, 0.5, 0.0 } },
        { 300, 500.0, { 0, 0, -1 }, { x, y, z + 250 }, { 1.0, 0.5, 0.0 } }
    };

    int numCones = sizeof(cones) / sizeof(cones[0]);

    for (int i = 0; i < numCones; i++)
    {
        vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
        coneSource->SetRadius(cones[i].radius);
        coneSource->SetHeight(cones[i].height);
        coneSource->SetDirection(cones[i].direction);
        coneSource->SetCenter(cones[i].center);
        coneSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        coneMapper->SetInputConnection(coneSource->GetOutputPort());

        vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
        coneActor->SetMapper(coneMapper);
        coneActor->GetProperty()->SetColor(cones[i].color);
        coneActor->GetProperty()->SetRepresentationToWireframe();

        renderer->AddActor(coneActor);
    }

    renderer->ResetCamera();
}
