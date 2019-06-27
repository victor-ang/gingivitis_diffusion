/****************************************************************************
** Meta object code from reading C++ file 'signalslotbase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/external/MecaCell/mecacellviewer/signalslotbase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signalslotbase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MecacellViewer__SignalSlotRenderer_t {
    QByteArrayData data[4];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MecacellViewer__SignalSlotRenderer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MecacellViewer__SignalSlotRenderer_t qt_meta_stringdata_MecacellViewer__SignalSlotRenderer = {
    {
QT_MOC_LITERAL(0, 0, 34), // "MecacellViewer::SignalSlotRen..."
QT_MOC_LITERAL(1, 35, 9), // "paintSlot"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 11) // "cleanupSlot"

    },
    "MecacellViewer::SignalSlotRenderer\0"
    "paintSlot\0\0cleanupSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MecacellViewer__SignalSlotRenderer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MecacellViewer::SignalSlotRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SignalSlotRenderer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->paintSlot(); break;
        case 1: _t->cleanupSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MecacellViewer::SignalSlotRenderer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MecacellViewer__SignalSlotRenderer.data,
    qt_meta_data_MecacellViewer__SignalSlotRenderer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MecacellViewer::SignalSlotRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MecacellViewer::SignalSlotRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MecacellViewer__SignalSlotRenderer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MecacellViewer::SignalSlotRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_MecacellViewer__SignalSlotBase_t {
    QByteArrayData data[29];
    char stringdata0[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MecacellViewer__SignalSlotBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MecacellViewer__SignalSlotBase_t qt_meta_stringdata_MecacellViewer__SignalSlotBase = {
    {
QT_MOC_LITERAL(0, 0, 30), // "MecacellViewer::SignalSlotBase"
QT_MOC_LITERAL(1, 31, 12), // "statsChanged"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 14), // "guiCtrlChanged"
QT_MOC_LITERAL(4, 60, 25), // "displayMenuElementToggled"
QT_MOC_LITERAL(5, 86, 1), // "l"
QT_MOC_LITERAL(6, 88, 1), // "c"
QT_MOC_LITERAL(7, 90, 11), // "buttonClick"
QT_MOC_LITERAL(8, 102, 4), // "name"
QT_MOC_LITERAL(9, 107, 4), // "sync"
QT_MOC_LITERAL(10, 112, 12), // "initRenderer"
QT_MOC_LITERAL(11, 125, 4), // "init"
QT_MOC_LITERAL(12, 130, 19), // "SignalSlotRenderer*"
QT_MOC_LITERAL(13, 150, 1), // "r"
QT_MOC_LITERAL(14, 152, 10), // "callUpdate"
QT_MOC_LITERAL(15, 163, 4), // "step"
QT_MOC_LITERAL(16, 168, 14), // "setWorldUpdate"
QT_MOC_LITERAL(17, 183, 1), // "u"
QT_MOC_LITERAL(18, 185, 19), // "handleWindowChanged"
QT_MOC_LITERAL(19, 205, 13), // "QQuickWindow*"
QT_MOC_LITERAL(20, 219, 3), // "win"
QT_MOC_LITERAL(21, 223, 10), // "getGuiCtrl"
QT_MOC_LITERAL(22, 234, 10), // "setGuiCtrl"
QT_MOC_LITERAL(23, 245, 1), // "s"
QT_MOC_LITERAL(24, 247, 8), // "getStats"
QT_MOC_LITERAL(25, 256, 7), // "getStat"
QT_MOC_LITERAL(26, 264, 8), // "setStats"
QT_MOC_LITERAL(27, 273, 7), // "guiCtrl"
QT_MOC_LITERAL(28, 281, 5) // "stats"

    },
    "MecacellViewer::SignalSlotBase\0"
    "statsChanged\0\0guiCtrlChanged\0"
    "displayMenuElementToggled\0l\0c\0buttonClick\0"
    "name\0sync\0initRenderer\0init\0"
    "SignalSlotRenderer*\0r\0callUpdate\0step\0"
    "setWorldUpdate\0u\0handleWindowChanged\0"
    "QQuickWindow*\0win\0getGuiCtrl\0setGuiCtrl\0"
    "s\0getStats\0getStat\0setStats\0guiCtrl\0"
    "stats"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MecacellViewer__SignalSlotBase[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       2,  136, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,  101,    2, 0x0a /* Public */,
       7,    1,  106,    2, 0x0a /* Public */,
       9,    0,  109,    2, 0x0a /* Public */,
      10,    0,  110,    2, 0x0a /* Public */,
      11,    1,  111,    2, 0x0a /* Public */,
      14,    0,  114,    2, 0x0a /* Public */,
      15,    0,  115,    2, 0x0a /* Public */,
      16,    1,  116,    2, 0x0a /* Public */,
      18,    1,  119,    2, 0x0a /* Public */,
      21,    0,  122,    2, 0x0a /* Public */,
      21,    1,  123,    2, 0x0a /* Public */,
      22,    1,  126,    2, 0x0a /* Public */,
      24,    0,  129,    2, 0x0a /* Public */,
      25,    1,  130,    2, 0x0a /* Public */,
      26,    1,  133,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant, QMetaType::Bool,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::QVariantMap,
    QMetaType::QVariant, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QVariantMap,   23,
    QMetaType::QVariantMap,
    QMetaType::QVariant, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QVariantMap,   23,

 // properties: name, type, flags
      27, QMetaType::QVariantMap, 0x00495103,
      28, QMetaType::QVariantMap, 0x00495103,

 // properties: notify_signal_id
       1,
       0,

       0        // eod
};

void MecacellViewer::SignalSlotBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SignalSlotBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->statsChanged(); break;
        case 1: _t->guiCtrlChanged(); break;
        case 2: _t->displayMenuElementToggled((*reinterpret_cast< QVariant(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->buttonClick((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sync(); break;
        case 5: _t->initRenderer(); break;
        case 6: _t->init((*reinterpret_cast< SignalSlotRenderer*(*)>(_a[1]))); break;
        case 7: _t->callUpdate(); break;
        case 8: _t->step(); break;
        case 9: _t->setWorldUpdate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->handleWindowChanged((*reinterpret_cast< QQuickWindow*(*)>(_a[1]))); break;
        case 11: { QVariantMap _r = _t->getGuiCtrl();
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 12: { QVariant _r = _t->getGuiCtrl((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->setGuiCtrl((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 14: { QVariantMap _r = _t->getStats();
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 15: { QVariant _r = _t->getStat((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->setStats((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SignalSlotRenderer* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SignalSlotBase::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SignalSlotBase::statsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SignalSlotBase::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SignalSlotBase::guiCtrlChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SignalSlotBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantMap*>(_v) = _t->getGuiCtrl(); break;
        case 1: *reinterpret_cast< QVariantMap*>(_v) = _t->getStats(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<SignalSlotBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setGuiCtrl(*reinterpret_cast< QVariantMap*>(_v)); break;
        case 1: _t->setStats(*reinterpret_cast< QVariantMap*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject MecacellViewer::SignalSlotBase::staticMetaObject = { {
    &QQuickItem::staticMetaObject,
    qt_meta_stringdata_MecacellViewer__SignalSlotBase.data,
    qt_meta_data_MecacellViewer__SignalSlotBase,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MecacellViewer::SignalSlotBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MecacellViewer::SignalSlotBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MecacellViewer__SignalSlotBase.stringdata0))
        return static_cast<void*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int MecacellViewer::SignalSlotBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MecacellViewer::SignalSlotBase::statsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MecacellViewer::SignalSlotBase::guiCtrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
