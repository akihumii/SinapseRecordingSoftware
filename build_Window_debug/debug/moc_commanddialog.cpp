/****************************************************************************
** Meta object code from reading C++ file 'commanddialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/commanddialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commanddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommandDialog_t {
    QByteArrayData data[17];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandDialog_t qt_meta_stringdata_CommandDialog = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CommandDialog"
QT_MOC_LITERAL(1, 14, 22), // "on_sendCommand_clicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 20), // "on_chipReset_clicked"
QT_MOC_LITERAL(4, 59, 17), // "on_SELALL_clicked"
QT_MOC_LITERAL(5, 77, 18), // "on_SELNONE_clicked"
QT_MOC_LITERAL(6, 96, 24), // "on_JTAGextension_clicked"
QT_MOC_LITERAL(7, 121, 20), // "on_JTAGreset_clicked"
QT_MOC_LITERAL(8, 142, 15), // "on_JTAG_toggled"
QT_MOC_LITERAL(9, 158, 15), // "on_Mode_Changed"
QT_MOC_LITERAL(10, 174, 4), // "Mode"
QT_MOC_LITERAL(11, 179, 17), // "on_ChipID_Changed"
QT_MOC_LITERAL(12, 197, 5), // "IDnum"
QT_MOC_LITERAL(13, 203, 17), // "on_SELCHN_toggled"
QT_MOC_LITERAL(14, 221, 17), // "on_BioImp_toggled"
QT_MOC_LITERAL(15, 239, 14), // "on_DCL_toggled"
QT_MOC_LITERAL(16, 254, 17) // "on_BER_textEdited"

    },
    "CommandDialog\0on_sendCommand_clicked\0"
    "\0on_chipReset_clicked\0on_SELALL_clicked\0"
    "on_SELNONE_clicked\0on_JTAGextension_clicked\0"
    "on_JTAGreset_clicked\0on_JTAG_toggled\0"
    "on_Mode_Changed\0Mode\0on_ChipID_Changed\0"
    "IDnum\0on_SELCHN_toggled\0on_BioImp_toggled\0"
    "on_DCL_toggled\0on_BER_textEdited"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    1,   86,    2, 0x08 /* Private */,
      11,    1,   89,    2, 0x08 /* Private */,
      13,    0,   92,    2, 0x08 /* Private */,
      14,    0,   93,    2, 0x08 /* Private */,
      15,    0,   94,    2, 0x08 /* Private */,
      16,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CommandDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommandDialog *_t = static_cast<CommandDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_sendCommand_clicked(); break;
        case 1: _t->on_chipReset_clicked(); break;
        case 2: _t->on_SELALL_clicked(); break;
        case 3: _t->on_SELNONE_clicked(); break;
        case 4: _t->on_JTAGextension_clicked(); break;
        case 5: _t->on_JTAGreset_clicked(); break;
        case 6: _t->on_JTAG_toggled(); break;
        case 7: _t->on_Mode_Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_ChipID_Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_SELCHN_toggled(); break;
        case 10: _t->on_BioImp_toggled(); break;
        case 11: _t->on_DCL_toggled(); break;
        case 12: _t->on_BER_textEdited(); break;
        default: ;
        }
    }
}

const QMetaObject CommandDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CommandDialog.data,
      qt_meta_data_CommandDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CommandDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CommandDialog.stringdata0))
        return static_cast<void*>(const_cast< CommandDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CommandDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
