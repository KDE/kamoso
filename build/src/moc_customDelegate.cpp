/****************************************************************************
** Meta object code from reading C++ file 'customDelegate.h'
**
** Created: Fri Oct 16 13:32:51 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/customDelegate.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customDelegate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomDelegate[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CustomDelegate[] = {
    "CustomDelegate\0\0it,idx,\0"
    "pixmapNeeded(KFileItem,QModelIndex,QRect)\0"
};

const QMetaObject CustomDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_CustomDelegate,
      qt_meta_data_CustomDelegate, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomDelegate::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomDelegate))
        return static_cast<void*>(const_cast< CustomDelegate*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int CustomDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pixmapNeeded((*reinterpret_cast< const KFileItem(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2])),(*reinterpret_cast< const QRect(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CustomDelegate::pixmapNeeded(const KFileItem & _t1, const QModelIndex & _t2, const QRect & _t3)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(const_cast< CustomDelegate *>(this), &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
