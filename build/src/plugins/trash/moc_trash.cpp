/****************************************************************************
** Meta object code from reading C++ file 'trash.h'
**
** Created: Fri Oct 16 13:32:45 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/trash/trash.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trash.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TrashPlugin[] = {

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
      13,   12,   12,   12, 0x0a,
      25,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TrashPlugin[] = {
    "TrashPlugin\0\0trash(bool)\0slotResult(KJob*)\0"
};

const QMetaObject TrashPlugin::staticMetaObject = {
    { &KamosoPlugin::staticMetaObject, qt_meta_stringdata_TrashPlugin,
      qt_meta_data_TrashPlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TrashPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TrashPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TrashPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrashPlugin))
        return static_cast<void*>(const_cast< TrashPlugin*>(this));
    if (!strcmp(_clname, "org.kamoso.plugin"))
        return static_cast< KamosoPlugin*>(const_cast< TrashPlugin*>(this));
    return KamosoPlugin::qt_metacast(_clname);
}

int TrashPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KamosoPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: trash((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: slotResult((*reinterpret_cast< KJob*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
