/****************************************************************************
** Meta object code from reading C++ file 'comboboxwidgetcolor.h'
**
** Created: Sun Aug 26 11:18:27 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "comboboxwidgetcolor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'comboboxwidgetcolor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ComboBoxWidgetColor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   51,   20,   20, 0x0a,
      84,   74,   20,   20, 0x0a,
     108,  102,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ComboBoxWidgetColor[] = {
    "ComboBoxWidgetColor\0\0newColor\0"
    "colorChanged(QColor)\0color\0setColor(QColor)\0"
    "colorName\0setColor(QString)\0index\0"
    "valueChanged(int)\0"
};

void ComboBoxWidgetColor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ComboBoxWidgetColor *_t = static_cast<ComboBoxWidgetColor *>(_o);
        switch (_id) {
        case 0: _t->colorChanged((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: _t->setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->setColor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ComboBoxWidgetColor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ComboBoxWidgetColor::staticMetaObject = {
    { &ComboBoxNameWidget::staticMetaObject, qt_meta_stringdata_ComboBoxWidgetColor,
      qt_meta_data_ComboBoxWidgetColor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ComboBoxWidgetColor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ComboBoxWidgetColor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ComboBoxWidgetColor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ComboBoxWidgetColor))
        return static_cast<void*>(const_cast< ComboBoxWidgetColor*>(this));
    return ComboBoxNameWidget::qt_metacast(_clname);
}

int ComboBoxWidgetColor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ComboBoxNameWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ComboBoxWidgetColor::colorChanged(QColor _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
