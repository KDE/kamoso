/****************************************************************************
** Meta object code from reading C++ file 'youtubejob.h'
**
** Created: Fri Oct 16 13:32:44 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/youtube/youtubejob.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'youtubejob.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_YoutubeJob[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x0a,
      47,   12,   11,   11, 0x0a,
      80,   12,   11,   11, 0x0a,
     111,   11,   11,   11, 0x0a,
     128,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_YoutubeJob[] = {
    "YoutubeJob\0\0,\0fileOpened(KIO::Job*,QByteArray)\0"
    "uploadDone(KIO::Job*,QByteArray)\0"
    "moreData(KIO::Job*,QByteArray)\0"
    "uploadNeedData()\0uploadFinal()\0"
};

const QMetaObject YoutubeJob::staticMetaObject = {
    { &KamosoJob::staticMetaObject, qt_meta_stringdata_YoutubeJob,
      qt_meta_data_YoutubeJob, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &YoutubeJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *YoutubeJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *YoutubeJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_YoutubeJob))
        return static_cast<void*>(const_cast< YoutubeJob*>(this));
    return KamosoJob::qt_metacast(_clname);
}

int YoutubeJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KamosoJob::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fileOpened((*reinterpret_cast< KIO::Job*(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 1: uploadDone((*reinterpret_cast< KIO::Job*(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: moreData((*reinterpret_cast< KIO::Job*(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: uploadNeedData(); break;
        case 4: uploadFinal(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
