/****************************************************************************
** Meta object code from reading C++ file 'qvv_view.h'
**
** Created: Fri Oct 16 15:47:07 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qvv_view.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qvv_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QvvView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      22,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QvvView[] = {
    "QvvView\0\0slotCenter()\0slotHelp()\0"
};

const QMetaObject QvvView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QvvView,
      qt_meta_data_QvvView, 0 }
};

const QMetaObject *QvvView::metaObject() const
{
    return &staticMetaObject;
}

void *QvvView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QvvView))
        return static_cast<void*>(const_cast< QvvView*>(this));
    return QWidget::qt_metacast(_clname);
}

int QvvView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotCenter(); break;
        case 1: slotHelp(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
