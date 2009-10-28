/****************************************************************************
** Meta object code from reading C++ file 'thumbnailview.h'
**
** Created: Fri Oct 16 13:32:47 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/thumbnailview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thumbnailview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ThumbnailView[] = {

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
      24,   15,   14,   14, 0x08,
      72,   60,   14,   14, 0x08,
     120,  116,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ThumbnailView[] = {
    "ThumbnailView\0\0file,pic\0"
    "previewAvailable(KFileItem,QPixmap)\0"
    "it,idx,rect\0retrievePixmap(KFileItem,QModelIndex,QRect)\0"
    "idx\0updatexClick(QModelIndex)\0"
};

const QMetaObject ThumbnailView::staticMetaObject = {
    { &QListView::staticMetaObject, qt_meta_stringdata_ThumbnailView,
      qt_meta_data_ThumbnailView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ThumbnailView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ThumbnailView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ThumbnailView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThumbnailView))
        return static_cast<void*>(const_cast< ThumbnailView*>(this));
    return QListView::qt_metacast(_clname);
}

int ThumbnailView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: previewAvailable((*reinterpret_cast< const KFileItem(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 1: retrievePixmap((*reinterpret_cast< const KFileItem(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2])),(*reinterpret_cast< const QRect(*)>(_a[3]))); break;
        case 2: updatexClick((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
