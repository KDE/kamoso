/****************************************************************************
** Meta object code from reading C++ file 'fbtalker.h'
**
** Created: Fri Oct 16 13:32:44 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/facebook/fbtalker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fbtalker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KIPIFacebookPlugin__FbTalker[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   30,   29,   29, 0x05,
      70,   51,   29,   29, 0x05,
     120,  107,   29,   29, 0x25,
     154,  149,   29,   29, 0x25,
     194,  179,   29,   29, 0x05,
     223,  179,   29,   29, 0x05,
     257,  179,   29,   29, 0x05,
     314,  289,   29,   29, 0x05,
     383,  357,   29,   29, 0x05,
     454,  428,   29,   29, 0x05,
     549,  523,   29,   29, 0x05,
     645,  618,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
     723,  714,   29,   29, 0x08,
     754,  750,   29,   29, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KIPIFacebookPlugin__FbTalker[] = {
    "KIPIFacebookPlugin::FbTalker\0\0val\0"
    "signalBusy(bool)\0step,maxStep,label\0"
    "signalLoginProgress(int,int,QString)\0"
    "step,maxStep\0signalLoginProgress(int,int)\0"
    "step\0signalLoginProgress(int)\0"
    "errCode,errMsg\0signalLoginDone(int,QString)\0"
    "signalChangePermDone(int,QString)\0"
    "signalAddPhotoDone(int,QString)\0"
    "errCode,errMsg,photoData\0"
    "signalGetPhotoDone(int,QString,QByteArray)\0"
    "errCode,errMsg,newAlbumID\0"
    "signalCreateAlbumDone(int,QString,long long)\0"
    "errCode,errMsg,albumsList\0"
    "signalListAlbumsDone(int,QString,QList<KIPIFacebookPlugin::FbAlbum>)\0"
    "errCode,errMsg,photosList\0"
    "signalListPhotosDone(int,QString,QList<KIPIFacebookPlugin::FbPhoto>)\0"
    "errCode,errMsg,friendsList\0"
    "signalListFriendsDone(int,QString,QList<KIPIFacebookPlugin::FbUser>)\0"
    "job,data\0data(KIO::Job*,QByteArray)\0"
    "job\0slotResult(KJob*)\0"
};

const QMetaObject KIPIFacebookPlugin::FbTalker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KIPIFacebookPlugin__FbTalker,
      qt_meta_data_KIPIFacebookPlugin__FbTalker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KIPIFacebookPlugin::FbTalker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KIPIFacebookPlugin::FbTalker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KIPIFacebookPlugin::FbTalker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KIPIFacebookPlugin__FbTalker))
        return static_cast<void*>(const_cast< FbTalker*>(this));
    return QObject::qt_metacast(_clname);
}

int KIPIFacebookPlugin::FbTalker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalBusy((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: signalLoginProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: signalLoginProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: signalLoginProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: signalLoginDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: signalChangePermDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: signalAddPhotoDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: signalGetPhotoDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 8: signalCreateAlbumDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< long long(*)>(_a[3]))); break;
        case 9: signalListAlbumsDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<KIPIFacebookPlugin::FbAlbum>(*)>(_a[3]))); break;
        case 10: signalListPhotosDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<KIPIFacebookPlugin::FbPhoto>(*)>(_a[3]))); break;
        case 11: signalListFriendsDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<KIPIFacebookPlugin::FbUser>(*)>(_a[3]))); break;
        case 12: data((*reinterpret_cast< KIO::Job*(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 13: slotResult((*reinterpret_cast< KJob*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void KIPIFacebookPlugin::FbTalker::signalBusy(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KIPIFacebookPlugin::FbTalker::signalLoginProgress(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 4
void KIPIFacebookPlugin::FbTalker::signalLoginDone(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void KIPIFacebookPlugin::FbTalker::signalChangePermDone(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void KIPIFacebookPlugin::FbTalker::signalAddPhotoDone(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void KIPIFacebookPlugin::FbTalker::signalGetPhotoDone(int _t1, const QString & _t2, const QByteArray & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void KIPIFacebookPlugin::FbTalker::signalCreateAlbumDone(int _t1, const QString & _t2, long long _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void KIPIFacebookPlugin::FbTalker::signalListAlbumsDone(int _t1, const QString & _t2, const QList<KIPIFacebookPlugin::FbAlbum> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void KIPIFacebookPlugin::FbTalker::signalListPhotosDone(int _t1, const QString & _t2, const QList<KIPIFacebookPlugin::FbPhoto> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void KIPIFacebookPlugin::FbTalker::signalListFriendsDone(int _t1, const QString & _t2, const QList<KIPIFacebookPlugin::FbUser> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
