/****************************************************************************
** Meta object code from reading C++ file 'timeplotwidget.h'
**
** Created: Thu Sep 13 07:34:18 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "timeplotwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'timeplotwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TimePlotWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   16,   15,   15, 0x0a,
      50,   41,   15,   15, 0x0a,
      70,   15,   15,   15, 0x0a,
      90,   15,   15,   15, 0x0a,
     113,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TimePlotWidget[] = {
    "TimePlotWidget\0\0SR\0setSampleRate(double)\0"
    "duration\0setDuration(double)\0"
    "timeOptionPressed()\0zoomPanButtonPressed()\0"
    "updatePlot()\0"
};

void TimePlotWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TimePlotWidget *_t = static_cast<TimePlotWidget *>(_o);
        switch (_id) {
        case 0: _t->setSampleRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->timeOptionPressed(); break;
        case 3: _t->zoomPanButtonPressed(); break;
        case 4: _t->updatePlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TimePlotWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TimePlotWidget::staticMetaObject = {
    { &PlotWidget::staticMetaObject, qt_meta_stringdata_TimePlotWidget,
      qt_meta_data_TimePlotWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TimePlotWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TimePlotWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TimePlotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TimePlotWidget))
        return static_cast<void*>(const_cast< TimePlotWidget*>(this));
    return PlotWidget::qt_metacast(_clname);
}

int TimePlotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PlotWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
