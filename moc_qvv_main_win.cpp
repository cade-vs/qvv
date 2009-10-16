/****************************************************************************
** Meta object code from reading C++ file 'qvv_main_win.h'
**
** Created: Fri Oct 16 11:36:37 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qvv_main_win.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qvv_main_win.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QvvTreeWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_QvvTreeWidget[] = {
    "QvvTreeWidget\0"
};

const QMetaObject QvvTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_QvvTreeWidget,
      qt_meta_data_QvvTreeWidget, 0 }
};

const QMetaObject *QvvTreeWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QvvTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QvvTreeWidget))
        return static_cast<void*>(const_cast< QvvTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int QvvTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_QvvMainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x0a,
      51,   44,   14,   14, 0x0a,
      77,   14,   14,   14, 0x0a,
     113,  101,   14,   14, 0x0a,
     153,   14,   14,   14, 0x0a,
     169,   14,   14,   14, 0x0a,
     180,   14,   14,   14, 0x0a,
     193,   14,   14,   14, 0x0a,
     209,   14,   14,   14, 0x0a,
     223,   14,   14,   14, 0x0a,
     239,   14,   14,   14, 0x0a,
     258,   14,   14,   14, 0x0a,
     271,   14,   14,   14, 0x0a,
     284,   14,   14,   14, 0x0a,
     302,   14,   14,   14, 0x0a,
     320,   14,   14,   14, 0x0a,
     337,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QvvMainWindow[] = {
    "QvvMainWindow\0\0item\0enter(QTreeWidgetItem*)\0"
    "action\0actionTriggered(QAction*)\0"
    "switchLayoutDirection()\0item,column\0"
    "slotItemActivated(QTreeWidgetItem*,int)\0"
    "slotNewWindow()\0slotGoUp()\0slotThumbs()\0"
    "slotChangeDir()\0slotHomeDir()\0"
    "slotReloadDir()\0slotShowDirsOnly()\0"
    "slotGoNext()\0slotGoPrev()\0slotSortColumn1()\0"
    "slotSortColumn3()\0slotRandomItem()\0"
    "slotHelp()\0"
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
        case 0: enter((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: actionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: switchLayoutDirection(); break;
        case 3: slotItemActivated((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: slotNewWindow(); break;
        case 5: slotGoUp(); break;
        case 6: slotThumbs(); break;
        case 7: slotChangeDir(); break;
        case 8: slotHomeDir(); break;
        case 9: slotReloadDir(); break;
        case 10: slotShowDirsOnly(); break;
        case 11: slotGoNext(); break;
        case 12: slotGoPrev(); break;
        case 13: slotSortColumn1(); break;
        case 14: slotSortColumn3(); break;
        case 15: slotRandomItem(); break;
        case 16: slotHelp(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
