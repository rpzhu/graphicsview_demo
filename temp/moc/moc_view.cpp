/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../chip/view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GraphicsView_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphicsView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphicsView_t qt_meta_stringdata_GraphicsView = {
    {
QT_MOC_LITERAL(0, 0, 12) // "GraphicsView"

    },
    "GraphicsView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphicsView[] = {

 // content:
       8,       // revision
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

void GraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject GraphicsView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_GraphicsView.data,
    qt_meta_data_GraphicsView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphicsView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int GraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_drilldownView_t {
    QByteArrayData data[14];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_drilldownView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_drilldownView_t qt_meta_stringdata_drilldownView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "drilldownView"
QT_MOC_LITERAL(1, 14, 6), // "zoomIn"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 5), // "level"
QT_MOC_LITERAL(4, 28, 7), // "zoomOut"
QT_MOC_LITERAL(5, 36, 9), // "resetView"
QT_MOC_LITERAL(6, 46, 21), // "setResetButtonEnabled"
QT_MOC_LITERAL(7, 68, 11), // "setupMatrix"
QT_MOC_LITERAL(8, 80, 17), // "togglePointerMode"
QT_MOC_LITERAL(9, 98, 12), // "toggleOpenGL"
QT_MOC_LITERAL(10, 111, 18), // "toggleAntialiasing"
QT_MOC_LITERAL(11, 130, 5), // "print"
QT_MOC_LITERAL(12, 136, 10), // "rotateLeft"
QT_MOC_LITERAL(13, 147, 11) // "rotateRight"

    },
    "drilldownView\0zoomIn\0\0level\0zoomOut\0"
    "resetView\0setResetButtonEnabled\0"
    "setupMatrix\0togglePointerMode\0"
    "toggleOpenGL\0toggleAntialiasing\0print\0"
    "rotateLeft\0rotateRight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_drilldownView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       1,    0,   82,    2, 0x2a /* Public | MethodCloned */,
       4,    1,   83,    2, 0x0a /* Public */,
       4,    0,   86,    2, 0x2a /* Public | MethodCloned */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void drilldownView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<drilldownView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomIn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->zoomIn(); break;
        case 2: _t->zoomOut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->zoomOut(); break;
        case 4: _t->resetView(); break;
        case 5: _t->setResetButtonEnabled(); break;
        case 6: _t->setupMatrix(); break;
        case 7: _t->togglePointerMode(); break;
        case 8: _t->toggleOpenGL(); break;
        case 9: _t->toggleAntialiasing(); break;
        case 10: _t->print(); break;
        case 11: _t->rotateLeft(); break;
        case 12: _t->rotateRight(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject drilldownView::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_drilldownView.data,
    qt_meta_data_drilldownView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *drilldownView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *drilldownView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_drilldownView.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int drilldownView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
