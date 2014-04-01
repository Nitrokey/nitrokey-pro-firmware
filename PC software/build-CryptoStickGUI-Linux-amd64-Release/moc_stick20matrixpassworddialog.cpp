/****************************************************************************
** Meta object code from reading C++ file 'stick20matrixpassworddialog.h'
**
** Created: Wed Mar 26 21:53:44 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/stick20matrixpassworddialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stick20matrixpassworddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MatrixPasswordDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      48,   21,   21,   21, 0x08,
      74,   21,   21,   21, 0x08,
     100,   21,   21,   21, 0x08,
     126,   21,   21,   21, 0x08,
     152,   21,   21,   21, 0x08,
     178,   21,   21,   21, 0x08,
     204,   21,   21,   21, 0x08,
     230,   21,   21,   21, 0x08,
     256,   21,   21,   21, 0x08,
     282,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MatrixPasswordDialog[] = {
    "MatrixPasswordDialog\0\0on_pushButton_0_clicked()\0"
    "on_pushButton_1_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "CheckGetPasswordMatrix()\0"
};

void MatrixPasswordDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MatrixPasswordDialog *_t = static_cast<MatrixPasswordDialog *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_0_clicked(); break;
        case 1: _t->on_pushButton_1_clicked(); break;
        case 2: _t->on_pushButton_2_clicked(); break;
        case 3: _t->on_pushButton_3_clicked(); break;
        case 4: _t->on_pushButton_4_clicked(); break;
        case 5: _t->on_pushButton_5_clicked(); break;
        case 6: _t->on_pushButton_6_clicked(); break;
        case 7: _t->on_pushButton_7_clicked(); break;
        case 8: _t->on_pushButton_8_clicked(); break;
        case 9: _t->on_pushButton_9_clicked(); break;
        case 10: _t->CheckGetPasswordMatrix(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MatrixPasswordDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MatrixPasswordDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MatrixPasswordDialog,
      qt_meta_data_MatrixPasswordDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MatrixPasswordDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MatrixPasswordDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MatrixPasswordDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MatrixPasswordDialog))
        return static_cast<void*>(const_cast< MatrixPasswordDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MatrixPasswordDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
