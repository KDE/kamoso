/****************************************************************************
** Meta object code from reading C++ file 'webcamwidget.h'
**
** Created: Fri Oct 16 13:32:45 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/webcamwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webcamwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebcamWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x0a,
      47,   42,   37,   13, 0x0a,
      69,   63,   13,   13, 0x0a,
      95,   87,   13,   13, 0x0a,
     115,   13,   13,   13, 0x0a,
     132,   42,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WebcamWidget[] = {
    "WebcamWidget\0\0file\0playFile(QString)\0"
    "bool\0dest\0takePhoto(KUrl)\0sound\0"
    "recordVideo(bool)\0destUrl\0stopRecording(KUrl)\0"
    "fileSaved(KJob*)\0fileSaved(KUrl)\0"
};

const QMetaObject WebcamWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WebcamWidget,
      qt_meta_data_WebcamWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebcamWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebcamWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebcamWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebcamWidget))
        return static_cast<void*>(const_cast< WebcamWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int WebcamWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: playFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: { bool _r = takePhoto((*reinterpret_cast< const KUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: recordVideo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: stopRecording((*reinterpret_cast< const KUrl(*)>(_a[1]))); break;
        case 4: fileSaved((*reinterpret_cast< KJob*(*)>(_a[1]))); break;
        case 5: fileSaved((*reinterpret_cast< const KUrl(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
