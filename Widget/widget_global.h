#ifndef WIDGET_GLOBAL_H
#define WIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WIDGET_LIBRARY)
#  define WIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WIDGET_GLOBAL_H
