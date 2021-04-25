#pragma once

#include "TlvObjectBase.hpp"

// #define ADD_PROP(name, type) AddProperty(name, globalTypes.TypeName(typeid(type)), &type)

// #define WRITE_BASIC(field) WriteBasicType(field, properties)
// #define WRITE_ENUMS(field) WriteEnumValue(types, properties, field)

// #define READ_BASIC(field) ReadBasicType(field, properties)
// #define READ_ENUMS(field) ReadEnumValue(types, field, properties)

#define TLVOBJECTBASE_ADD(name, prop) AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), static_cast<void*>(&prop), true)
#define TLVOBJECTBASE_ADD_HIDDEN(name, prop) AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), static_cast<void*>(&prop), false)

// #define TLVOBJECTBASE_COPY_TLV() if (pTlv) { mTlv = *reinterpret_cast<decltype(&mTlv)>(pTlv); }

