/****************************************************************************
** Meta object code from reading C++ file 'selectcurvewindowdialog.h'
**
** Created: Tue Aug 28 20:33:45 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "selectcurvewindowdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selectcurvewindowdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SelectCurveWindowDialog[] = {

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
      31,   25,   24,   24, 0x0a,
      49,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SelectCurveWindowDialog[] = {
    "SelectCurveWindowDialog\0\0index\0"
    "changedCurve(int)\0cancelSelect()\0"
};

void SelectCurveWindowDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SelectCurveWindowDialog *_t = static_cast<SelectCurveWindowDialog *>(_o);
        switch (_id) {
        case 0: _t->changedCurve((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->cancelSelect(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SelectCurveWindowDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SelectCurveWindowDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SelectCurveWindowDialog,
      qt_meta_data_SelectCurveWindowDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SelectCurveWindowDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SelectCurveWindowDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SelectCurveWindowDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SelectCurveWindowDialog))
        return static_cast<void*>(const_cast< SelectCurveWindowDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SelectCurveWindowDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
