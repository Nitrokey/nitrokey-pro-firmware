/****************************************************************************
** Meta object code from reading C++ file 'stick20hiddenvolumedialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/stick20hiddenvolumedialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stick20hiddenvolumedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_stick20HiddenVolumeDialog[] = {

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
      35,   27,   26,   26, 0x08,
      73,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_stick20HiddenVolumeDialog[] = {
    "stick20HiddenVolumeDialog\0\0checked\0"
    "on_ShowPasswordCheckBox_toggled(bool)\0"
    "on_buttonBox_accepted()\0"
};

void stick20HiddenVolumeDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        stick20HiddenVolumeDialog *_t = static_cast<stick20HiddenVolumeDialog *>(_o);
        switch (_id) {
        case 0: _t->on_ShowPasswordCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_buttonBox_accepted(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData stick20HiddenVolumeDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject stick20HiddenVolumeDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_stick20HiddenVolumeDialog,
      qt_meta_data_stick20HiddenVolumeDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &stick20HiddenVolumeDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *stick20HiddenVolumeDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *stick20HiddenVolumeDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_stick20HiddenVolumeDialog))
        return static_cast<void*>(const_cast< stick20HiddenVolumeDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int stick20HiddenVolumeDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
