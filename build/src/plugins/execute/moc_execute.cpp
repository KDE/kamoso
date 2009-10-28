/****************************************************************************
** Meta object code from reading C++ file 'execute.h'
**
** Created: Fri Oct 16 13:32:43 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/execute/execute.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'execute.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExecutePlugin[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExecutePlugin[] = {
    "ExecutePlugin\0\0execute(bool)\0"
};

const QMetaObject ExecutePlugin::staticMetaObject = {
    { &KamosoPlugin::staticMetaObject, qt_meta_stringdata_ExecutePlugin,
      qt_meta_data_ExecutePlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExecutePlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExecutePlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExecutePlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExecutePlugin))
        return static_cast<void*>(const_cast< ExecutePlugin*>(this));
    if (!strcmp(_clname, "org.kamoso.plugin"))
        return static_cast< KamosoPlugin*>(const_cast< ExecutePlugin*>(this));
    return KamosoPlugin::qt_metacast(_clname);
}

int ExecutePlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KamosoPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: execute((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
