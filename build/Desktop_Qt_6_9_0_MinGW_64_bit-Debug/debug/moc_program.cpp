/****************************************************************************
** Meta object code from reading C++ file 'program.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../program.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'program.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN7programE_t {};
} // unnamed namespace

template <> constexpr inline auto program::qt_create_metaobjectdata<qt_meta_tag_ZN7programE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "program",
        "input",
        "",
        "update_output",
        "QString&",
        "str",
        "clean",
        "update_trees",
        "update_code",
        "Clear",
        "Quit",
        "Help"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'input'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'update_output'
        QtMocHelpers::SignalData<void(QString &, bool)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 }, { QMetaType::Bool, 6 },
        }}),
        // Signal 'update_trees'
        QtMocHelpers::SignalData<void(QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'update_code'
        QtMocHelpers::SignalData<void(QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'Clear'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'Quit'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'Help'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<program, qt_meta_tag_ZN7programE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject program::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7programE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7programE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7programE_t>.metaTypes,
    nullptr
} };

void program::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<program *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->input(); break;
        case 1: _t->update_output((*reinterpret_cast< std::add_pointer_t<QString&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->update_trees((*reinterpret_cast< std::add_pointer_t<QString&>>(_a[1]))); break;
        case 3: _t->update_code((*reinterpret_cast< std::add_pointer_t<QString&>>(_a[1]))); break;
        case 4: _t->Clear(); break;
        case 5: _t->Quit(); break;
        case 6: _t->Help(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (program::*)()>(_a, &program::input, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (program::*)(QString & , bool )>(_a, &program::update_output, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (program::*)(QString & )>(_a, &program::update_trees, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (program::*)(QString & )>(_a, &program::update_code, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (program::*)()>(_a, &program::Clear, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (program::*)()>(_a, &program::Quit, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (program::*)()>(_a, &program::Help, 6))
            return;
    }
}

const QMetaObject *program::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *program::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7programE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int program::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void program::input()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void program::update_output(QString & _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void program::update_trees(QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void program::update_code(QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void program::Clear()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void program::Quit()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void program::Help()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
