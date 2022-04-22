#pragma once

#define ADD_RESOURCE(resAnimId, type) AddResource(resAnimId, type);
#define ADD(name, prop) AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), static_cast<void*>(&prop), true)
#define ADD_HIDDEN(name, prop) AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), static_cast<void*>(&prop), false)
