#include "BaseGameObject.hpp"
#include "../AliveLibAE/BaseGameObject.hpp"
#include "../AliveLibAO/BaseGameObject.hpp"

ReliveTypes IBaseGameObject::FromAO(AO::AOTypes aoType)
{
    switch (aoType)
    {
        case AO::AOTypes::eAbe_43:
            return ReliveTypes::eAbe;
    }
    return ReliveTypes::eNone;
}

AO::AOTypes IBaseGameObject::ToAO(ReliveTypes reliveType)
{
    switch (reliveType)
    {
        case ReliveTypes::eAbe:
            return AO::AOTypes::eAbe_43;
    }
    return AO::AOTypes::eNone_0;
}

ReliveTypes IBaseGameObject::FromAE(AETypes aeType)
{
    switch (aeType)
    {
        case AETypes::eGlukkon_67:
            return ReliveTypes::eGlukkon;
    }
    return ReliveTypes::eNone;
}

AETypes IBaseGameObject::ToAE(ReliveTypes reliveType)
{
    switch (reliveType)
    {
        case ReliveTypes::eGlukkon:
            return AETypes::eGlukkon_67;
    }
    return AETypes::eNone_0;
}
