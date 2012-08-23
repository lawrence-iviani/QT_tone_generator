/****************************************************************************
** Meta object code from reading C++ file 'generictimedata.h'
**
** Created: Fri Aug 24 00:14:14 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "generictimedata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'generictimedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GenericTimeData[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      43,   31,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   31,   16,   16, 0x0a,
      95,   92,   16,   16, 0x0a,
     122,  117,   16,   16, 0x0a,
     145,  139,   16,   16, 0x0a,
     169,  162,   16,   16, 0x0a,
     190,   16,   16,   16, 0x0a,
     204,   16,   16,   16, 0x0a,
     217,   16,   16,   16, 0x0a,
     235,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GenericTimeData[] = {
    "GenericTimeData\0\0dataUpdated()\0"
    "maxDuration\0maxDurationUpdate(double)\0"
    "setMaxDuration(double)\0SR\0"
    "setSampleRate(double)\0name\0setName(QString)\0"
    "color\0setColor(QColor)\0enable\0"
    "setEnableCurve(bool)\0nameUpdated()\0"
    "updateData()\0showBaseControl()\0"
    "showExtendedControl()\0"
};

void GenericTimeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GenericTimeData *_t = static_cast<GenericTimeData *>(_o);
        switch (_id) {
        case 0: _t->dataUpdated(); break;
        case 1: _t->maxDurationUpdate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setMaxDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setSampleRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 6: _t->setEnableCurve((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->nameUpdated(); break;
        case 8: _t->updateData(); break;
        case 9: _t->showBaseControl(); break;
        case 10: _t->showExtendedControl(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GenericTimeData::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GenericTimeData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GenericTimeData,
      qt_meta_data_GenericTimeData, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GenericTimeData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GenericTimeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GenericTimeData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenericTimeData))
        return static_cast<void*>(const_cast< GenericTimeData*>(this));
    return QObject::qt_metacast(_clname);
}

int GenericTimeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GenericTimeData::dataUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GenericTimeData::maxDurationUpdate(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
