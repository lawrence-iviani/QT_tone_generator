/****************************************************************************
** Meta object code from reading C++ file 'generictimedata.h'
**
** Created: Mon Aug 20 13:13:17 2012
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
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   31,   16,   16, 0x0a,
      63,   60,   16,   16, 0x0a,
      97,   85,   16,   16, 0x0a,
     139,  136,   16,   16, 0x0a,
     163,  160,   16,   16, 0x0a,
     187,  182,   16,   16, 0x0a,
     210,  204,   16,   16, 0x0a,
     234,  227,   16,   16, 0x0a,
     255,   16,   16,   16, 0x0a,
     269,   16,   16,   16, 0x0a,
     282,   16,   16,   16, 0x0a,
     300,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GenericTimeData[] = {
    "GenericTimeData\0\0dataUpdated()\0duration\0"
    "setDuration(double)\0SR\0setSampleRate(double)\0"
    "t0,duration\0setStartTimeAndDuration(double,double)\0"
    "t0\0setStartTime(double)\0t1\0"
    "setEndTime(double)\0name\0setName(QString)\0"
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
        case 1: _t->setDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setSampleRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setStartTimeAndDuration((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->setStartTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setEndTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 8: _t->setEnableCurve((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->nameUpdated(); break;
        case 10: _t->updateData(); break;
        case 11: _t->showBaseControl(); break;
        case 12: _t->showExtendedControl(); break;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void GenericTimeData::dataUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
