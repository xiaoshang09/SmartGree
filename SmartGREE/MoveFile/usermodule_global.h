#pragma once

#include <QtCore/qglobal.h>

#ifdef BUILD_LIB
# if defined(USERMODULE_LIB)
#  define USERMODULE_EXPORT Q_DECL_EXPORT
# else
#  define USERMODULE_EXPORT Q_DECL_IMPORT
# endif
#else
# define USERMODULE_EXPORT
#endif
