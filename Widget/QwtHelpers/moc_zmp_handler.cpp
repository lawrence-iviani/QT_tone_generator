/****************************************************************************
** Meta object code from reading C++ file 'zmp_handler.h'
**
** Created: Sun Aug 26 11:18:29 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "zmp_handler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'zmp_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ZMP_Handler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      28,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ZMP_Handler[] = {
    "ZMP_Handler\0\0enableZoomer()\0enablePanner()\0"
};

void ZMP_Handler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ZMP_Handler *_t = static_cast<ZMP_Handler *>(_o);
        switch (_id) {
        case 0: _t->enableZoomer(); break;
        case 1: _t->enablePanner(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ZMP_Handler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ZMP_Handler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ZMP_Handler,
      qt_meta_data_ZMP_Handler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ZMP_Handler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ZMP_Handler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ZMP_Handler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ZMP_Handler))
        return static_cast<void*>(const_cast< ZMP_Handler*>(this));
    return QObject::qt_metacast(_clname);
}

int ZMP_Handler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
