/****************************************************************************
** Meta object code from reading C++ file 'smtpp.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../smtpp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smtpp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_smtpp_t {
    uint offsetsAndSizes[24];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_smtpp_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_smtpp_t qt_meta_stringdata_smtpp = {
    {
        QT_MOC_LITERAL(0, 5),  // "smtpp"
        QT_MOC_LITERAL(6, 6),  // "status"
        QT_MOC_LITERAL(13, 0),  // ""
        QT_MOC_LITERAL(14, 12),  // "stateChanged"
        QT_MOC_LITERAL(27, 28),  // "QAbstractSocket::SocketState"
        QT_MOC_LITERAL(56, 11),  // "socketState"
        QT_MOC_LITERAL(68, 13),  // "errorReceived"
        QT_MOC_LITERAL(82, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(111, 11),  // "socketError"
        QT_MOC_LITERAL(123, 12),  // "disconnected"
        QT_MOC_LITERAL(136, 9),  // "connected"
        QT_MOC_LITERAL(146, 9)   // "readyRead"
    },
    "smtpp\0status\0\0stateChanged\0"
    "QAbstractSocket::SocketState\0socketState\0"
    "errorReceived\0QAbstractSocket::SocketError\0"
    "socketError\0disconnected\0connected\0"
    "readyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_smtpp[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   53,    2, 0x08,    3 /* Private */,
       6,    1,   56,    2, 0x08,    5 /* Private */,
       9,    0,   59,    2, 0x08,    7 /* Private */,
      10,    0,   60,    2, 0x08,    8 /* Private */,
      11,    0,   61,    2, 0x08,    9 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void smtpp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<smtpp *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->status((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketState>>(_a[1]))); break;
        case 2: _t->errorReceived((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->connected(); break;
        case 5: _t->readyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketState >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (smtpp::*)(const QString & );
            if (_t _q_method = &smtpp::status; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject smtpp::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_smtpp.offsetsAndSizes,
    qt_meta_data_smtpp,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_smtpp_t
, QtPrivate::TypeAndForceComplete<smtpp, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketState, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *smtpp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *smtpp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_smtpp.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int smtpp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void smtpp::status(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
