/****************************************************************************
** Meta object code from reading C++ file 'hotpdialog.h'
**
** Created: Fri Dec 27 14:41:05 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/hotpdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hotpdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HOTPDialog[] = {

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
      12,   11,   11,   11, 0x08,
      36,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HOTPDialog[] = {
    "HOTPDialog\0\0on_nextButton_clicked()\0"
    "on_clipboardButton_clicked()\0"
};

void HOTPDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HOTPDialog *_t = static_cast<HOTPDialog *>(_o);
        switch (_id) {
        case 0: _t->on_nextButton_clicked(); break;
        case 1: _t->on_clipboardButton_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData HOTPDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HOTPDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_HOTPDialog,
      qt_meta_data_HOTPDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HOTPDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HOTPDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HOTPDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HOTPDialog))
        return static_cast<void*>(const_cast< HOTPDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int HOTPDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
