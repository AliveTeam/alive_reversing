#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>
#include <QComboBox>
#include "../../../relive_lib/data_conversion/relive_tlvs.hpp"
#include <array>

class QUndoStack;


template<typename EnumType> struct EnumReflector final { };

#define REFLECT_ENUM(ENUM_TYPE, ...)                                                    \
   template<>                                                                           \
   struct EnumReflector<ENUM_TYPE> final                                                \
   {                                                                                    \
        static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!");  \
        struct EnumPair final                                                           \
        {                                                                               \
           ENUM_TYPE mValue;                                                            \
           const char* mName;                                                           \
        };                                                                              \
                                                                                        \
        static s32 to_index(ENUM_TYPE e)                                                \
        {                                                                               \
            s32 idx = 0;                                                                \
            for (auto& v : mArray)                                                      \
            {                                                                           \
                if (v.mValue == e)                                                      \
                {                                                                       \
                    return idx;                                                         \
                }                                                                       \
                idx++;                                                                  \
            }                                                                           \
            return -1;                                                                  \
        }                                                                               \
                                                                                        \
        static const char* to_str(s32 idx)                                              \
        {                                                                               \
            return mArray[idx].mName;                                                   \
        }                                                                               \
                                                                                        \
        static ENUM_TYPE to_value(s32 idx)                                              \
        {                                                                               \
            return mArray[idx].mValue;                                                  \
        }                                                                               \
       static constexpr EnumPair tmp[] = __VA_ARGS__;                                   \
       static constexpr std::array<EnumPair, ALIVE_COUNTOF(tmp)> mArray = {__VA_ARGS__};\
   };


// TODO: Move to another header
REFLECT_ENUM(relive::reliveScale, {
            {relive::reliveScale::eFull, "eFull"},
            {relive::reliveScale::eHalf, "eHalf"}
             })

REFLECT_ENUM(relive::Path_Mudokon::Mud_TLV_Emotion, {
                 {relive::Path_Mudokon::Mud_TLV_Emotion::eNormal, "Normal"},
                             {relive::Path_Mudokon::Mud_TLV_Emotion::eAngry, "Angry"},
                             {relive::Path_Mudokon::Mud_TLV_Emotion::eSad, "Sad"},
                             {relive::Path_Mudokon::Mud_TLV_Emotion::eWired, "Wired"},
                             {relive::Path_Mudokon::Mud_TLV_Emotion::eSick, "Sick"}})


struct EnumPropertyChangeData
{
    EnumPropertyChangeData(const char* oldEnumName, const char* newEnumName, int oldIdx, int newIdx)
        : mOldEnumName(oldEnumName), mNewEnumName(newEnumName), mOldIdx(oldIdx), mNewIdx(newIdx)
    {

    }

    const char* mOldEnumName = nullptr;
    const char* mNewEnumName = nullptr;
    int mOldIdx = 0;
    int mNewIdx = 0;
};

// TODO: Merge with above ?
class EnumPropertyBase;
class LinkedEnumProperty
{
public:
    LinkedEnumProperty(PropertyTreeWidget* pTree, const char* pFieldName, void* pEnumKey, IGraphicsItem* pGraphicsItem)
     : mTree(pTree), mFieldName(pFieldName), mEnumKey(pEnumKey), mGraphicsItem(pGraphicsItem)
    {

    }

    PropertyTreeWidget* mTree = nullptr;
    const char* mFieldName = nullptr;
    void* mEnumKey = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
};

class ChangeEnumPropertyCommand : public QUndoCommand
{
public:
    ChangeEnumPropertyCommand(LinkedEnumProperty linkedProperty, EnumPropertyChangeData propertyData);

    void undo() override;

    void redo() override;

private:
    LinkedEnumProperty mLinkedProperty;
    EnumPropertyChangeData mPropertyData;
};

class EnumPropertyBase : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    EnumPropertyBase(s32 oldIdx, void* pEnumValue, const char* fieldName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem);

    QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    void Refresh() override;

    const void* GetPropertyLookUpKey() const override
    {
        return mEnumVale;
    }

    virtual const char* VGetEnumName(s32 idx) const = 0;
    virtual void VPopulateEnumCombo(QComboBox* pCombo) = 0;
    virtual void VSetEnumFromIdx(s32 idx) = 0;
protected:
    s32 mOldIdx = 0;
private:
    void* mEnumVale = nullptr;
    const char* mFieldName;
    QUndoStack& mUndoStack;
    IGraphicsItem* mGraphicsItem = nullptr;
    QComboBox* mCombo = nullptr;
};

template<typename EnumType>
class EnumProperty final : public EnumPropertyBase
{
public:
    EnumProperty(EnumType& enumValue, const char* fieldName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem)
     : EnumPropertyBase(EnumReflector<EnumType>::to_index(enumValue), &enumValue, fieldName, undoStack, pGraphicsItem)
     , mEnumValue(enumValue)
    {

    }

    const char* VGetEnumName(s32 idx) const override
    {
        return EnumReflector<EnumType>::to_str(idx);
    }

    void VPopulateEnumCombo(QComboBox* pCombo) override
    {
        for (auto& item : EnumReflector<EnumType>::mArray)
        {
            pCombo->addItem(item.mName);
        }
    }

    void VSetEnumFromIdx(s32 idx) override
    {
        mOldIdx = idx;
        mEnumValue = EnumReflector<EnumType>::to_value(idx);
    }
private:
    EnumType& mEnumValue;
};
