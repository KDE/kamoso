/****************************************************************************
** Meta object code from reading C++ file 'kamoso.h'
**
** Created: Tue Oct 27 20:15:06 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/kamoso.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kamoso.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Kamoso[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,    7,    7,    7, 0x0a,
      55,   43,    7,    7, 0x0a,
      77,    7,    7,    7, 0x2a,
      94,    7,    7,    7, 0x0a,
     110,    7,    7,    7, 0x0a,
     133,  127,    7,    7, 0x0a,
     152,    7,    7,    7, 0x0a,
     166,    7,    7,    7, 0x0a,
     192,  182,    7,    7, 0x0a,
     232,    7,    7,    7, 0x0a,
     249,    7,    7,    7, 0x0a,
     262,    7,    7,    7, 0x0a,
     281,    7,    7,    7, 0x0a,
     303,    7,    7,    7, 0x0a,
     320,    7,    7,    7, 0x0a,
     344,  341,    7,    7, 0x0a,
     366,    7,    7,    7, 0x0a,
     382,    7,    7,    7, 0x08,
     392,    7,    7,    7, 0x08,
     409,    7,    7,    7, 0x08,
     427,    7,    7,    7, 0x08,
     450,  446,    7,    7, 0x08,
     481,  477,    7,    7, 0x08,
     508,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Kamoso[] = {
    "Kamoso\0\0webcamPlaying(QString)\0"
    "takePhoto()\0minimumTime\0startCountdown(qreal)\0"
    "startCountdown()\0configuration()\0"
    "generalUpdated()\0index\0webcamChanged(int)\0"
    "webcamAdded()\0webcamRemoved()\0item,menu\0"
    "contextMenuThumbnails(KFileItem,QMenu*)\0"
    "thumbnailAdded()\0selectLast()\0"
    "settingsMenu(bool)\0selectJob(KamosoJob*)\0"
    "changeMode(bool)\0webcamValueChanged()\0"
    "en\0setFlashEnabled(bool)\0stopCountdown()\0"
    "restore()\0slotScrollLeft()\0slotScrollRight()\0"
    "slotScrollFinish()\0idx\0"
    "openThumbnail(QModelIndex)\0url\0"
    "openThumbnail(QList<KUrl>)\0"
    "fillKcomboDevice()\0"
};

const QMetaObject Kamoso::staticMetaObject = {
    { &KMainWindow::staticMetaObject, qt_meta_stringdata_Kamoso,
      qt_meta_data_Kamoso, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Kamoso::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Kamoso::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Kamoso::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Kamoso))
        return static_cast<void*>(const_cast< Kamoso*>(this));
    return KMainWindow::qt_metacast(_clname);
}

int Kamoso::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: webcamPlaying((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: takePhoto(); break;
        case 2: startCountdown((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 3: startCountdown(); break;
        case 4: configuration(); break;
        case 5: generalUpdated(); break;
        case 6: webcamChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: webcamAdded(); break;
        case 8: webcamRemoved(); break;
        case 9: contextMenuThumbnails((*reinterpret_cast< const KFileItem(*)>(_a[1])),(*reinterpret_cast< QMenu*(*)>(_a[2]))); break;
        case 10: thumbnailAdded(); break;
        case 11: selectLast(); break;
        case 12: settingsMenu((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: selectJob((*reinterpret_cast< KamosoJob*(*)>(_a[1]))); break;
        case 14: changeMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: webcamValueChanged(); break;
        case 16: setFlashEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: stopCountdown(); break;
        case 18: restore(); break;
        case 19: slotScrollLeft(); break;
        case 20: slotScrollRight(); break;
        case 21: slotScrollFinish(); break;
        case 22: openThumbnail((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 23: openThumbnail((*reinterpret_cast< const QList<KUrl>(*)>(_a[1]))); break;
        case 24: fillKcomboDevice(); break;
        default: ;
        }
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void Kamoso::webcamPlaying(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
