/****************************************************************************
** Meta object code from reading C++ file 'photoshootmode.h'
**
** Created: Fri Oct 16 13:32:49 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/photoshootmode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'photoshootmode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhotoShootMode[] = {

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
      24,   16,   15,   15, 0x08,
      43,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PhotoShootMode[] = {
    "PhotoShootMode\0\0pressed\0shootClicked(bool)\0"
    "release()\0"
};

const QMetaObject PhotoShootMode::staticMetaObject = {
    { &ShootMode::staticMetaObject, qt_meta_stringdata_PhotoShootMode,
      qt_meta_data_PhotoShootMode, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhotoShootMode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhotoShootMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhotoShootMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhotoShootMode))
        return static_cast<void*>(const_cast< PhotoShootMode*>(this));
    return ShootMode::qt_metacast(_clname);
}

int PhotoShootMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ShootMode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: shootClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: release(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
