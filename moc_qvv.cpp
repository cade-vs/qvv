/****************************************************************************
** Meta object code from reading C++ file 'qvv.h'
**
** Created: Sat Oct 10 20:27:31 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qvv.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qvv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QvvMainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      22,   15,   14,   14, 0x0a,
      48,   14,   14,   14, 0x0a,
      72,   14,   14,   14, 0x0a,
      88,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QvvMainWindow[] = {
    "QvvMainWindow\0\0action\0actionTriggered(QAction*)\0"
    "switchLayoutDirection()\0slotNewWindow()\0"
    "slotGoUp()\0"
};

const QMetaObject QvvMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QvvMainWindow,
      qt_meta_data_QvvMainWindow, 0 }
};

const QMetaObject *QvvMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *QvvMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QvvMainWindow))
        return static_cast<void*>(const_cast< QvvMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QvvMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: actionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: switchLayoutDirection(); break;
        case 2: slotNewWindow(); break;
        case 3: slotGoUp(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
