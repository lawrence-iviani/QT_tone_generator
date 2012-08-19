/****************************************************************************
** Meta object code from reading C++ file 'scaledslider.h'
**
** Created: Mon Jun 11 22:04:10 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "scaledslider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scaledslider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ScaledSlider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       1,   29, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   44,   13,   13, 0x0a,
      83,   77,   13,   13, 0x08,

 // enums: name, flags, count, data
     110, 0x0,    2,   33,

 // enum data: key, value
     116, uint(ScaledSlider::Linear),
     123, uint(ScaledSlider::Logarithmic),

       0        // eod
};

static const char qt_meta_stringdata_ScaledSlider[] = {
    "ScaledSlider\0\0convertedValueChanged(double)\0"
    "val\0setNotConvertedValue(double)\0value\0"
    "convertSliderValue(double)\0Scale\0"
    "Linear\0Logarithmic\0"
};

void ScaledSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ScaledSlider *_t = static_cast<ScaledSlider *>(_o);
        switch (_id) {
        case 0: _t->convertedValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setNotConvertedValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->convertSliderValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ScaledSlider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ScaledSlider::staticMetaObject = {
    { &QwtSlider::staticMetaObject, qt_meta_stringdata_ScaledSlider,
      qt_meta_data_ScaledSlider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ScaledSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ScaledSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ScaledSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScaledSlider))
        return static_cast<void*>(const_cast< ScaledSlider*>(this));
    return QwtSlider::qt_metacast(_clname);
}

int ScaledSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ScaledSlider::convertedValueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
