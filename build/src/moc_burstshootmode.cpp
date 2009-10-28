/****************************************************************************
** Meta object code from reading C++ file 'burstshootmode.h'
**
** Created: Fri Oct 16 13:32:49 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/burstshootmode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'burstshootmode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BurstShootMode[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      35,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BurstShootMode[] = {
    "BurstShootMode\0\0stateChanged(bool)\0"
    "keepTaking()\0"
};

const QMetaObject BurstShootMode::staticMetaObject = {
    { &ShootMode::staticMetaObject, qt_meta_stringdata_BurstShootMode,
      qt_meta_data_BurstShootMode, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BurstShootMode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BurstShootMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BurstShootMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BurstShootMode))
        return static_cast<void*>(const_cast< BurstShootMode*>(this));
    return ShootMode::qt_metacast(_clname);
}

int BurstShootMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ShootMode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: keepTaking(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
