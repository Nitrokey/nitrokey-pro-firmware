/****************************************************************************
** Meta object code from reading C++ file 'stick20setup.h'
**
** Created: Mon Feb 24 21:36:51 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/stick20setup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stick20setup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Stick20Setup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      53,   13,   13,   13, 0x08,
      83,   13,   13,   13, 0x08,
     118,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Stick20Setup[] = {
    "Stick20Setup\0\0on_pushButton_Change_AdminPW_clicked()\0"
    "on_pushButton_Ch_PW_clicked()\0"
    "on_pushButton_Ch_Mat_APW_clicked()\0"
    "on_pushButton_Ch_HiddenVol_clicked()\0"
};

void Stick20Setup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Stick20Setup *_t = static_cast<Stick20Setup *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_Change_AdminPW_clicked(); break;
        case 1: _t->on_pushButton_Ch_PW_clicked(); break;
        case 2: _t->on_pushButton_Ch_Mat_APW_clicked(); break;
        case 3: _t->on_pushButton_Ch_HiddenVol_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Stick20Setup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Stick20Setup::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Stick20Setup,
      qt_meta_data_Stick20Setup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Stick20Setup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Stick20Setup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Stick20Setup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Stick20Setup))
        return static_cast<void*>(const_cast< Stick20Setup*>(this));
    return QDialog::qt_metacast(_clname);
}

int Stick20Setup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
