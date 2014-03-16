/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Feb 24 21:36:44 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CryptoStickGUI/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      58,   51,   11,   11, 0x08,
      75,   11,   11,   11, 0x08,
      99,   11,   11,   11, 0x08,
     125,   11,   11,   11, 0x08,
     150,   11,   11,   11, 0x08,
     177,   11,   11,   11, 0x08,
     213,  207,   11,   11, 0x08,
     254,   11,   11,   11, 0x08,
     287,  279,   11,   11, 0x08,
     319,  279,   11,   11, 0x08,
     354,   11,   11,   11, 0x08,
     383,   11,   11,   11, 0x08,
     414,  279,   11,   11, 0x08,
     442,  279,   11,   11, 0x08,
     475,   11,   11,   11, 0x08,
     528,  518,  513,   11, 0x08,
     565,  558,   11,   11, 0x08,
     614,   11,   11,   11, 0x08,
     625,   11,   11,   11, 0x08,
     636,   11,   11,   11, 0x08,
     647,   11,   11,   11, 0x08,
     658,   11,   11,   11, 0x08,
     669,   11,   11,   11, 0x08,
     680,   11,   11,   11, 0x08,
     705,   11,   11,   11, 0x08,
     743,   11,   11,   11, 0x08,
     775,  279,   11,   11, 0x08,
     801,   11,   11,   11, 0x08,
     829,   11,   11,   11, 0x08,
     847,   11,   11,   11, 0x08,
     875,   11,   11,   11, 0x08,
     895,   11,   11,   11, 0x08,
     929,   11,   11,   11, 0x08,
     964,   11,   11,   11, 0x08,
     997,   11,   11,   11, 0x08,
    1031,   11,   11,   11, 0x08,
    1066,   11,   11,   11, 0x08,
    1101,   11,   11,   11, 0x08,
    1136,   11,   11,   11, 0x08,
    1176,   11,   11,   11, 0x08,
    1205,   11,   11,   11, 0x08,
    1246,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0checkConnection()\0"
    "startConfiguration()\0slotNo\0"
    "getCode(uint8_t)\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_writeButton_clicked()\0"
    "displayCurrentSlotConfig()\0"
    "displayCurrentGeneralConfig()\0index\0"
    "on_slotComboBox_currentIndexChanged(int)\0"
    "on_resetButton_clicked()\0checked\0"
    "on_hexRadioButton_toggled(bool)\0"
    "on_base32RadioButton_toggled(bool)\0"
    "on_setToZeroButton_clicked()\0"
    "on_setToRandomButton_clicked()\0"
    "on_checkBox_2_toggled(bool)\0"
    "on_tokenIDCheckBox_toggled(bool)\0"
    "on_writeGeneralConfigButton_clicked()\0"
    "bool\0obj,event\0eventFilter(QObject*,QEvent*)\0"
    "reason\0iconActivated(QSystemTrayIcon::ActivationReason)\0"
    "getHOTP1()\0getHOTP2()\0getTOTP1()\0"
    "getTOTP2()\0getTOTP3()\0getTOTP4()\0"
    "on_eraseButton_clicked()\0"
    "on_resetGeneralConfigButton_clicked()\0"
    "on_randomSecretButton_clicked()\0"
    "on_checkBox_toggled(bool)\0"
    "startStick20Configuration()\0"
    "startStickDebug()\0startMatrixPasswordDialog()\0"
    "startStick20Setup()\0"
    "startStick20EnableCryptedVolume()\0"
    "startStick20DisableCryptedVolume()\0"
    "startStick20EnableHiddenVolume()\0"
    "startStick20DisableHiddenVolume()\0"
    "startStick20EnableFirmwareUpdate()\0"
    "startStick20ExportFirmwareToFile()\0"
    "startStick20DestroyCryptedVolume()\0"
    "startStick20FillSDCardWithRandomChars()\0"
    "startStick20GetStickStatus()\0"
    "startStick20SetReadonlyUncryptedVolume()\0"
    "startStick20SetReadWriteUncryptedVolume()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->checkConnection(); break;
        case 1: _t->startConfiguration(); break;
        case 2: _t->getCode((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->on_pushButton_2_clicked(); break;
        case 5: _t->on_writeButton_clicked(); break;
        case 6: _t->displayCurrentSlotConfig(); break;
        case 7: _t->displayCurrentGeneralConfig(); break;
        case 8: _t->on_slotComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_resetButton_clicked(); break;
        case 10: _t->on_hexRadioButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_base32RadioButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_setToZeroButton_clicked(); break;
        case 13: _t->on_setToRandomButton_clicked(); break;
        case 14: _t->on_checkBox_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->on_tokenIDCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->on_writeGeneralConfigButton_clicked(); break;
        case 17: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: _t->iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 19: _t->getHOTP1(); break;
        case 20: _t->getHOTP2(); break;
        case 21: _t->getTOTP1(); break;
        case 22: _t->getTOTP2(); break;
        case 23: _t->getTOTP3(); break;
        case 24: _t->getTOTP4(); break;
        case 25: _t->on_eraseButton_clicked(); break;
        case 26: _t->on_resetGeneralConfigButton_clicked(); break;
        case 27: _t->on_randomSecretButton_clicked(); break;
        case 28: _t->on_checkBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->startStick20Configuration(); break;
        case 30: _t->startStickDebug(); break;
        case 31: _t->startMatrixPasswordDialog(); break;
        case 32: _t->startStick20Setup(); break;
        case 33: _t->startStick20EnableCryptedVolume(); break;
        case 34: _t->startStick20DisableCryptedVolume(); break;
        case 35: _t->startStick20EnableHiddenVolume(); break;
        case 36: _t->startStick20DisableHiddenVolume(); break;
        case 37: _t->startStick20EnableFirmwareUpdate(); break;
        case 38: _t->startStick20ExportFirmwareToFile(); break;
        case 39: _t->startStick20DestroyCryptedVolume(); break;
        case 40: _t->startStick20FillSDCardWithRandomChars(); break;
        case 41: _t->startStick20GetStickStatus(); break;
        case 42: _t->startStick20SetReadonlyUncryptedVolume(); break;
        case 43: _t->startStick20SetReadWriteUncryptedVolume(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
