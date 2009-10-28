/****************************************************************************
** Meta object code from reading C++ file 'kamosojobtracker.h'
**
** Created: Fri Oct 16 13:32:46 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/kamosojobtracker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kamosojobtracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KamosoJobTracker[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   18,   17,   17, 0x0a,
      69,   18,   17,   17, 0x0a,
      96,   17,   90,   17, 0x0a,
     107,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KamosoJobTracker[] = {
    "KamosoJobTracker\0\0job\0jobClicked(KamosoJob*)\0"
    "registerJob(KamosoJob*)\0unregisterJob(KJob*)\0"
    "QSize\0sizeHint()\0paintEvent(QPaintEvent*)\0"
};

const QMetaObject KamosoJobTracker::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_KamosoJobTracker,
      qt_meta_data_KamosoJobTracker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KamosoJobTracker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KamosoJobTracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KamosoJobTracker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KamosoJobTracker))
        return static_cast<void*>(const_cast< KamosoJobTracker*>(this));
    return QWidget::qt_metacast(_clname);
}

int KamosoJobTracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: jobClicked((*reinterpret_cast< KamosoJob*(*)>(_a[1]))); break;
        case 1: registerJob((*reinterpret_cast< KamosoJob*(*)>(_a[1]))); break;
        case 2: unregisterJob((*reinterpret_cast< KJob*(*)>(_a[1]))); break;
        case 3: { QSize _r = sizeHint();
            if (_a[0]) *reinterpret_cast< QSize*>(_a[0]) = _r; }  break;
        case 4: paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KamosoJobTracker::jobClicked(KamosoJob * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
