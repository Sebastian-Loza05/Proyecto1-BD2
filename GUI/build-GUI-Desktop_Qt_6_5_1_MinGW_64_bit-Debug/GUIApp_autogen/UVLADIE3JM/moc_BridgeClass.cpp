/****************************************************************************
** Meta object code from reading C++ file 'BridgeClass.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GUI/src/BridgeClass.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BridgeClass.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSBridgeClassENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSBridgeClassENDCLASS = QtMocHelpers::stringData(
    "BridgeClass",
    "errorOccurred",
    "",
    "dataChanged",
    "runQuery",
    "query",
    "input",
    "std::vector<std::string>",
    "string",
    "ingreso",
    "verifyLogin",
    "username",
    "password",
    "getErrorMessage",
    "readRecords"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSBridgeClassENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[12];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[9];
    char stringdata5[6];
    char stringdata6[6];
    char stringdata7[25];
    char stringdata8[7];
    char stringdata9[8];
    char stringdata10[12];
    char stringdata11[9];
    char stringdata12[9];
    char stringdata13[16];
    char stringdata14[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSBridgeClassENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSBridgeClassENDCLASS_t qt_meta_stringdata_CLASSBridgeClassENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "BridgeClass"
        QT_MOC_LITERAL(12, 13),  // "errorOccurred"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 11),  // "dataChanged"
        QT_MOC_LITERAL(39, 8),  // "runQuery"
        QT_MOC_LITERAL(48, 5),  // "query"
        QT_MOC_LITERAL(54, 5),  // "input"
        QT_MOC_LITERAL(60, 24),  // "std::vector<std::string>"
        QT_MOC_LITERAL(85, 6),  // "string"
        QT_MOC_LITERAL(92, 7),  // "ingreso"
        QT_MOC_LITERAL(100, 11),  // "verifyLogin"
        QT_MOC_LITERAL(112, 8),  // "username"
        QT_MOC_LITERAL(121, 8),  // "password"
        QT_MOC_LITERAL(130, 15),  // "getErrorMessage"
        QT_MOC_LITERAL(146, 11)   // "readRecords"
    },
    "BridgeClass",
    "errorOccurred",
    "",
    "dataChanged",
    "runQuery",
    "query",
    "input",
    "std::vector<std::string>",
    "string",
    "ingreso",
    "verifyLogin",
    "username",
    "password",
    "getErrorMessage",
    "readRecords"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSBridgeClassENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,
       3,    0,   57,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   58,    2, 0x0a,    3 /* Public */,
       6,    1,   61,    2, 0x0a,    5 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      10,    2,   64,    2, 0x02,    7 /* Public */,
      13,    0,   69,    2, 0x102,   10 /* Public | MethodIsConst  */,
      14,    0,   70,    2, 0x102,   11 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    0x80000000 | 7, 0x80000000 | 8,    9,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   11,   12,
    QMetaType::QString,
    QMetaType::QString,

       0        // eod
};

Q_CONSTINIT const QMetaObject BridgeClass::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSBridgeClassENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSBridgeClassENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSBridgeClassENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BridgeClass, std::true_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'dataChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runQuery'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'input'
        QtPrivate::TypeAndForceComplete<std::vector<std::string>, std::false_type>,
        QtPrivate::TypeAndForceComplete<const string, std::false_type>,
        // method 'verifyLogin'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getErrorMessage'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'readRecords'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void BridgeClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BridgeClass *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->errorOccurred(); break;
        case 1: _t->dataChanged(); break;
        case 2: _t->runQuery((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: { std::vector<std::string> _r = _t->input((*reinterpret_cast< std::add_pointer_t<string>>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::vector<std::string>*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->verifyLogin((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->getErrorMessage();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->readRecords();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BridgeClass::*)();
            if (_t _q_method = &BridgeClass::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BridgeClass::*)();
            if (_t _q_method = &BridgeClass::dataChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *BridgeClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BridgeClass::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSBridgeClassENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BridgeClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void BridgeClass::errorOccurred()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BridgeClass::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
