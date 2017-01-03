/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CatmarkSubdiv/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[344];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 20), // "on_ImportOBJ_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 27), // "on_SubdivSteps_valueChanged"
QT_MOC_LITERAL(4, 61, 5), // "value"
QT_MOC_LITERAL(5, 67, 22), // "on_wireFrameCB_toggled"
QT_MOC_LITERAL(6, 90, 7), // "checked"
QT_MOC_LITERAL(7, 98, 24), // "on_limitPointsCB_toggled"
QT_MOC_LITERAL(8, 123, 22), // "on_quadPatchCB_toggled"
QT_MOC_LITERAL(9, 146, 24), // "on_controlMeshCB_toggled"
QT_MOC_LITERAL(10, 171, 28), // "on_innerLevelSB_valueChanged"
QT_MOC_LITERAL(11, 200, 4), // "arg1"
QT_MOC_LITERAL(12, 205, 28), // "on_outerLevelSB_valueChanged"
QT_MOC_LITERAL(13, 234, 22), // "on_gridLinesCB_toggled"
QT_MOC_LITERAL(14, 257, 22), // "on_showModelCB_toggled"
QT_MOC_LITERAL(15, 280, 28), // "on_applySharpnessPB_released"
QT_MOC_LITERAL(16, 309, 34) // "on_sharpnessSlider_editingFin..."

    },
    "MainWindow\0on_ImportOBJ_clicked\0\0"
    "on_SubdivSteps_valueChanged\0value\0"
    "on_wireFrameCB_toggled\0checked\0"
    "on_limitPointsCB_toggled\0"
    "on_quadPatchCB_toggled\0on_controlMeshCB_toggled\0"
    "on_innerLevelSB_valueChanged\0arg1\0"
    "on_outerLevelSB_valueChanged\0"
    "on_gridLinesCB_toggled\0on_showModelCB_toggled\0"
    "on_applySharpnessPB_released\0"
    "on_sharpnessSlider_editingFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    1,   75,    2, 0x08 /* Private */,
       5,    1,   78,    2, 0x08 /* Private */,
       7,    1,   81,    2, 0x08 /* Private */,
       8,    1,   84,    2, 0x08 /* Private */,
       9,    1,   87,    2, 0x08 /* Private */,
      10,    1,   90,    2, 0x08 /* Private */,
      12,    1,   93,    2, 0x08 /* Private */,
      13,    1,   96,    2, 0x08 /* Private */,
      14,    1,   99,    2, 0x08 /* Private */,
      15,    0,  102,    2, 0x08 /* Private */,
      16,    0,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_ImportOBJ_clicked(); break;
        case 1: _t->on_SubdivSteps_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_wireFrameCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_limitPointsCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_quadPatchCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_controlMeshCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_innerLevelSB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_outerLevelSB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_gridLinesCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_showModelCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_applySharpnessPB_released(); break;
        case 11: _t->on_sharpnessSlider_editingFinished(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
