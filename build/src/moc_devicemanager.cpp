/****************************************************************************
** Meta object code from reading C++ file 'devicemanager.h'
**
** Created: Sat Oct 17 07:06:12 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/devicemanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DeviceManager[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x05,
      45,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   15,   14,   14, 0x0a,
      96,   15,   14,   14, 0x08,
     117,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DeviceManager[] = {
    "DeviceManager\0\0udi\0deviceRegistered(QString)\0"
    "deviceUnregistered(QString)\0"
    "webcamPlaying(QString)\0deviceAdded(QString)\0"
    "deviceRemoved(QString)\0"
};

const QMetaObject DeviceManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeviceManager,
      qt_meta_data_DeviceManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DeviceManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DeviceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DeviceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceManager))
        return static_cast<void*>(const_cast< DeviceManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DeviceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: deviceRegistered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: deviceUnregistered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: webcamPlaying((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: deviceAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: deviceRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DeviceManager::deviceRegistered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DeviceManager::deviceUnregistered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
