/****************************************************************************
** Meta object code from reading C++ file 'freqplotwidget.h'
**
** Created: Fri Aug 24 00:14:17 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "freqplotwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'freqplotwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FreqPlotWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FreqPlotWidget[] = {
    "FreqPlotWidget\0\0freqOptionPressed()\0"
};

void FreqPlotWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FreqPlotWidget *_t = static_cast<FreqPlotWidget *>(_o);
        switch (_id) {
        case 0: _t->freqOptionPressed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FreqPlotWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FreqPlotWidget::staticMetaObject = {
    { &PlotWidget::staticMetaObject, qt_meta_stringdata_FreqPlotWidget,
      qt_meta_data_FreqPlotWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FreqPlotWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FreqPlotWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FreqPlotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FreqPlotWidget))
        return static_cast<void*>(const_cast< FreqPlotWidget*>(this));
    return PlotWidget::qt_metacast(_clname);
}

int FreqPlotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PlotWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
