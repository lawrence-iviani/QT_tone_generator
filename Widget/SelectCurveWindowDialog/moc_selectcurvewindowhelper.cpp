/****************************************************************************
** Meta object code from reading C++ file 'selectcurvewindowhelper.h'
**
** Created: Sun Jul 22 18:17:09 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "selectcurvewindowhelper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selectcurvewindowhelper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SelectCurveWindowHelper[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   39,   24,   24, 0x0a,
      71,   65,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SelectCurveWindowHelper[] = {
    "SelectCurveWindowHelper\0\0dataChanged()\0"
    "data\0addData(S_DataCurve)\0index\0"
    "setDataCurveIndex(int)\0"
};

void SelectCurveWindowHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SelectCurveWindowHelper *_t = static_cast<SelectCurveWindowHelper *>(_o);
        switch (_id) {
        case 0: _t->dataChanged(); break;
        case 1: _t->addData((*reinterpret_cast< S_DataCurve(*)>(_a[1]))); break;
        case 2: _t->setDataCurveIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SelectCurveWindowHelper::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SelectCurveWindowHelper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SelectCurveWindowHelper,
      qt_meta_data_SelectCurveWindowHelper, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SelectCurveWindowHelper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SelectCurveWindowHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SelectCurveWindowHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SelectCurveWindowHelper))
        return static_cast<void*>(const_cast< SelectCurveWindowHelper*>(this));
    return QObject::qt_metacast(_clname);
}

int SelectCurveWindowHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void SelectCurveWindowHelper::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
