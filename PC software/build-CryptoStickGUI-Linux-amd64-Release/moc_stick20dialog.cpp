/****************************************************************************
** Meta object code from reading C++ file 'stick20dialog.h'
**
** Created: Wed Mar 26 21:53:37 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/stick20dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stick20dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Stick20Dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      45,   39,   14,   14, 0x08,
      90,   82,   14,   14, 0x08,
     116,   82,   14,   14, 0x08,
     154,  149,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Stick20Dialog[] = {
    "Stick20Dialog\0\0on_buttonBox_accepted()\0"
    "index\0on_comboBox_currentIndexChanged(int)\0"
    "checked\0on_checkBox_toggled(bool)\0"
    "on_checkBox_Matrix_toggled(bool)\0arg1\0"
    "on_PasswordEdit_textChanged(QString)\0"
};

void Stick20Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Stick20Dialog *_t = static_cast<Stick20Dialog *>(_o);
        switch (_id) {
        case 0: _t->on_buttonBox_accepted(); break;
        case 1: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_checkBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_checkBox_Matrix_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_PasswordEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Stick20Dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Stick20Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Stick20Dialog,
      qt_meta_data_Stick20Dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Stick20Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Stick20Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Stick20Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Stick20Dialog))
        return static_cast<void*>(const_cast< Stick20Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Stick20Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
