/****************************************************************************
** Meta object code from reading C++ file 'stick20changepassworddialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/stick20changepassworddialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stick20changepassworddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialogChangePassword[] = {

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
      30,   22,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DialogChangePassword[] = {
    "DialogChangePassword\0\0checked\0"
    "on_checkBox_clicked(bool)\0"
};

void DialogChangePassword::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DialogChangePassword *_t = static_cast<DialogChangePassword *>(_o);
        switch (_id) {
        case 0: _t->on_checkBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DialogChangePassword::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DialogChangePassword::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DialogChangePassword,
      qt_meta_data_DialogChangePassword, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialogChangePassword::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialogChangePassword::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialogChangePassword::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogChangePassword))
        return static_cast<void*>(const_cast< DialogChangePassword*>(this));
    return QDialog::qt_metacast(_clname);
}

int DialogChangePassword::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
