
#ifndef USE_VTK_H
#define USE_VTK_H

#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

#ifdef _DEBUG
#include "F:\danpc\毕业论文\VTK_9.0\VTK\lib_debug\comment_lib.txt"
#else
#include "F:\danpc\毕业论文\VTK_9.0\VTK\lib_release\comment_lib.txt"
#endif // DEBUG


#endif //USE_VTK_H