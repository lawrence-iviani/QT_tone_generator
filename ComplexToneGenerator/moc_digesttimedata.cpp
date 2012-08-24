/****************************************************************************
** Meta object code from reading C++ file 'digesttimedata.h'
**
** Created: Sat Aug 25 00:16:06 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "digesttimedata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'digesttimedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DigestTimeData[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   16,   15,   15, 0x0a,
      54,   51,   15,   15, 0x0a,
      88,   76,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DigestTimeData[] = {
    "DigestTimeData\0\0maxDuration\0"
    "setMaxDuration(double)\0SR\0"
    "setSampleRate(double)\0m_curveList\0"
    "setTimeDataList(QList<GenericTimeData*>*)\0"
};

void DigestTimeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DigestTimeData *_t = static_cast<DigestTimeData *>(_o);
        switch (_id) {
        case 0: _t->setMaxDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setSampleRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setTimeDataList((*reinterpret_cast< QList<GenericTimeData*>*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DigestTimeData::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DigestTimeData::staticMetaObject = {
    { &GenericTimeData::staticMetaObject, qt_meta_stringdata_DigestTimeData,
      qt_meta_data_DigestTimeData, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DigestTimeData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DigestTimeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DigestTimeData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DigestTimeData))
        return static_cast<void*>(const_cast< DigestTimeData*>(this));
    return GenericTimeData::qt_metacast(_clname);
}

int DigestTimeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GenericTimeData::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
