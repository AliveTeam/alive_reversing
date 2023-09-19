#pragma once

#include "TlvObjectBaseRelive.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "../../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../../relive_lib/AnimResources.hpp"

#define CTOR_RELIVE(className, objectTypeName, tlvEnumType)\
    className() : TlvObjectBaseRelive(sizeof(::className), tlvEnumType, objectTypeName, &mTlv)\
    {\
    }\
    \
    className(ReliveAPI::TypesCollectionBase& globalTypes, const relive::Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseRelive(sizeof(::className), globalTypes, tlvEnumType, objectTypeName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const ::className*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->mLength = static_cast<s16>(mSizeOfT);\
        }\
        AddProperties(globalTypes);\
    }\
    ::className mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_RELIVE() (void) globalTypes

namespace ReliveTlvs {

struct Path_TimedMine final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_TimedMine, "TimedMine", ReliveTypes::eTimedMine)
    {
        ADD("Switch ID (Unused?)", mTlv.mSwitchId);
        // TODO
        /*
        ADD("State (Unused?)", mTlv.mState);
        ADD("Scale", mTlv.mScale);
        ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        */
    }
};


} // namespace ReliveTlvs

#undef EMPTY_CTOR_RELIVE
#undef CTOR_RELIVE
