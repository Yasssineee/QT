/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[362];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 18), // "on_ajouter_clicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 22), // "on_tableView_activated"
QT_MOC_LITERAL(4, 54, 5), // "index"
QT_MOC_LITERAL(5, 60, 19), // "on_modifier_clicked"
QT_MOC_LITERAL(6, 80, 14), // "navigateToPage"
QT_MOC_LITERAL(7, 95, 9), // "pageIndex"
QT_MOC_LITERAL(8, 105, 11), // "exportToPDF"
QT_MOC_LITERAL(9, 117, 13), // "exportToExcel"
QT_MOC_LITERAL(10, 131, 16), // "Rechercherclient"
QT_MOC_LITERAL(11, 148, 4), // "text"
QT_MOC_LITERAL(12, 153, 35), // "on_sortComboBox_currentIndexC..."
QT_MOC_LITERAL(13, 189, 28), // "displayCharts_clientByGender"
QT_MOC_LITERAL(14, 218, 17), // "on_gostat_clicked"
QT_MOC_LITERAL(15, 236, 28), // "on_calendrier_reserv_clicked"
QT_MOC_LITERAL(16, 265, 4), // "date"
QT_MOC_LITERAL(17, 270, 29), // "updateCalendarAndEventDetails"
QT_MOC_LITERAL(18, 300, 21), // "on_calendrier_clicked"
QT_MOC_LITERAL(19, 322, 15), // "on_Back_clicked"
QT_MOC_LITERAL(20, 338, 23) // "on_supprimerbtn_clicked"

    },
    "MainWindow\0on_ajouter_clicked\0\0"
    "on_tableView_activated\0index\0"
    "on_modifier_clicked\0navigateToPage\0"
    "pageIndex\0exportToPDF\0exportToExcel\0"
    "Rechercherclient\0text\0"
    "on_sortComboBox_currentIndexChanged\0"
    "displayCharts_clientByGender\0"
    "on_gostat_clicked\0on_calendrier_reserv_clicked\0"
    "date\0updateCalendarAndEventDetails\0"
    "on_calendrier_clicked\0on_Back_clicked\0"
    "on_supprimerbtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    1,   94,    2, 0x08 /* Private */,
       8,    0,   97,    2, 0x08 /* Private */,
       9,    0,   98,    2, 0x08 /* Private */,
      10,    1,   99,    2, 0x08 /* Private */,
      12,    1,  102,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    1,  107,    2, 0x08 /* Private */,
      17,    1,  110,    2, 0x08 /* Private */,
      18,    0,  113,    2, 0x08 /* Private */,
      19,    0,  114,    2, 0x08 /* Private */,
      20,    0,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   16,
    QMetaType::Void, QMetaType::QDate,   16,
    QMetaType::Void,
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
        case 0: _t->on_ajouter_clicked(); break;
        case 1: _t->on_tableView_activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->on_modifier_clicked(); break;
        case 3: _t->navigateToPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->exportToPDF(); break;
        case 5: _t->exportToExcel(); break;
        case 6: _t->Rechercherclient((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_sortComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->displayCharts_clientByGender(); break;
        case 9: _t->on_gostat_clicked(); break;
        case 10: _t->on_calendrier_reserv_clicked((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 11: _t->updateCalendarAndEventDetails((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 12: _t->on_calendrier_clicked(); break;
        case 13: _t->on_Back_clicked(); break;
        case 14: _t->on_supprimerbtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
