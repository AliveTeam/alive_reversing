#pragma once

#define ADD(name, prop)        AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), static_cast<void*>(&prop), true)
#define ADD_HIDDEN(name, prop) AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), static_cast<void*>(&prop), false)
#define COPY_TLV()             if (pTlv) { mTlv = *reinterpret_cast<decltype(&mTlv)>(pTlv); }
