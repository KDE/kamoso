/****************************************************************************
** Meta object code from reading C++ file 'kamosojob.h'
**
** Created: Fri Oct 16 12:23:31 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/kamosojob.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kamosojob.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KamosoJob[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_KamosoJob[] = {
    "KamosoJob\0"
};

const QMetaObject KamosoJob::staticMetaObject = {
    { &KJob::staticMetaObject, qt_meta_stringdata_KamosoJob,
      qt_meta_data_KamosoJob, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KamosoJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KamosoJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KamosoJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KamosoJob))
        return static_cast<void*>(const_cast< KamosoJob*>(this));
    return KJob::qt_metacast(_clname);
}

int KamosoJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KJob::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
