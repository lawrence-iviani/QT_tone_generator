/****************************************************************************
** Meta object code from reading C++ file 'sinusdata.h'
**
** Created: Mon Aug 20 18:03:04 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sinusdata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sinusdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SinusData[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      41,   11,   10,   10, 0x0a,
     103,   93,   10,   10, 0x0a,
     134,  124,   10,   10, 0x0a,
     165,  155,   10,   10, 0x0a,
     195,  186,   10,   10, 0x0a,
     218,  215,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SinusData[] = {
    "SinusData\0\0amplitude,frequency,initPhase\0"
    "setAmplitudeFrequencyAndPhase(double,double,double)\0"
    "amplitude\0setAmplitude(double)\0frequency\0"
    "setFrequency(double)\0initPhase\0"
    "setInitPhase(double)\0duration\0"
    "setDuration(double)\0t0\0setStartTime(double)\0"
};

void SinusData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SinusData *_t = static_cast<SinusData *>(_o);
        switch (_id) {
        case 0: _t->setAmplitudeFrequencyAndPhase((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 1: _t->setAmplitude((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setFrequency((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setInitPhase((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setStartTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SinusData::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SinusData::staticMetaObject = {
    { &GenericTimeData::staticMetaObject, qt_meta_stringdata_SinusData,
      qt_meta_data_SinusData, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SinusData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SinusData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SinusData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SinusData))
        return static_cast<void*>(const_cast< SinusData*>(this));
    return GenericTimeData::qt_metacast(_clname);
}

int SinusData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GenericTimeData::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
