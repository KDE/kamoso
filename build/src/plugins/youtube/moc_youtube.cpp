/****************************************************************************
** Meta object code from reading C++ file 'youtube.h'
**
** Created: Fri Oct 16 13:32:45 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/youtube/youtube.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'youtube.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_YoutubePlugin[] = {

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
      15,   14,   14,   14, 0x0a,
      24,   14,   14,   14, 0x0a,
      53,   44,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_YoutubePlugin[] = {
    "YoutubePlugin\0\0upload()\0authenticated(bool)\0"
    "job,data\0loginDone(KIO::Job*,QByteArray)\0"
};

const QMetaObject YoutubePlugin::staticMetaObject = {
    { &KamosoPlugin::staticMetaObject, qt_meta_stringdata_YoutubePlugin,
      qt_meta_data_YoutubePlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &YoutubePlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *YoutubePlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *YoutubePlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_YoutubePlugin))
        return static_cast<void*>(const_cast< YoutubePlugin*>(this));
    if (!strcmp(_clname, "org.kamoso.plugin"))
        return static_cast< KamosoPlugin*>(const_cast< YoutubePlugin*>(this));
    return KamosoPlugin::qt_metacast(_clname);
}

int YoutubePlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KamosoPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: upload(); break;
        case 1: authenticated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: loginDone((*reinterpret_cast< KIO::Job*(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
