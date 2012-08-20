/****************************************************************************
** Meta object code from reading C++ file 'plotwidget.h'
**
** Created: Mon Aug 20 23:33:22 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "plotwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlotWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      11, 0x0,    2,   18,

 // enum data: key, value
      17, uint(PlotWidget::Linear),
      24, uint(PlotWidget::Logarithmic),

       0        // eod
};

static const char qt_meta_stringdata_PlotWidget[] = {
    "PlotWidget\0Scale\0Linear\0Logarithmic\0"
};

void PlotWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PlotWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PlotWidget::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_PlotWidget,
      qt_meta_data_PlotWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlotWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlotWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotWidget))
        return static_cast<void*>(const_cast< PlotWidget*>(this));
    return QwtPlot::qt_metacast(_clname);
}

int PlotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
