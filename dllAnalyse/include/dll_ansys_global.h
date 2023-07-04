#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DLL_ANSYS_LIB)
#  define DLL_ANSYS_EXPORT Q_DECL_EXPORT
# else
#  define DLL_ANSYS_EXPORT Q_DECL_IMPORT
# endif
#else
# define DLL_ANSYS_EXPORT
#endif
