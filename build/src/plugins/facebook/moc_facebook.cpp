/****************************************************************************
** Meta object code from reading C++ file 'facebook.h'
**
** Created: Fri Oct 16 13:32:44 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/facebook/facebook.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'facebook.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FacebookPlugin[] = {

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
      16,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FacebookPlugin[] = {
    "FacebookPlugin\0\0uploadImage(bool)\0"
};

const QMetaObject FacebookPlugin::staticMetaObject = {
    { &KamosoPlugin::staticMetaObject, qt_meta_stringdata_FacebookPlugin,
      qt_meta_data_FacebookPlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FacebookPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FacebookPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FacebookPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FacebookPlugin))
        return static_cast<void*>(const_cast< FacebookPlugin*>(this));
    if (!strcmp(_clname, "org.kamoso.plugin"))
        return static_cast< KamosoPlugin*>(const_cast< FacebookPlugin*>(this));
    return KamosoPlugin::qt_metacast(_clname);
}

int FacebookPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KamosoPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: uploadImage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_FacebookJob[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   13,   12,   12, 0x08,
      76,   74,   12,   12, 0x08,
      99,   13,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FacebookJob[] = {
    "FacebookJob\0\0,,\0"
    "albumList(int,QString,QList<KIPIFacebookPlugin::FbAlbum>)\0"
    ",\0loginDone(int,QString)\0"
    "albumCreated(int,QString,long long)\0"
};

const QMetaObject FacebookJob::staticMetaObject = {
    { &KamosoJob::staticMetaObject, qt_meta_stringdata_FacebookJob,
      qt_meta_data_FacebookJob, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FacebookJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FacebookJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FacebookJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FacebookJob))
        return static_cast<void*>(const_cast< FacebookJob*>(this));
    return KamosoJob::qt_metacast(_clname);
}

int FacebookJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KamosoJob::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: albumList((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<KIPIFacebookPlugin::FbAlbum>(*)>(_a[3]))); break;
        case 1: loginDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: albumCreated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< long long(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
