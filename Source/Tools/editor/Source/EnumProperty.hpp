#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>
#include <QComboBox>
#include "ReflectedEnumProperties.hpp"

class QUndoStack;

struct EnumPropertyChangeData
{
    EnumPropertyChangeData(QString oldEnumName, QString newEnumName, int oldIdx, int newIdx)
        : mOldEnumName(oldEnumName), mNewEnumName(newEnumName), mOldIdx(oldIdx), mNewIdx(newIdx)
    {

    }

    QString mOldEnumName;
    QString mNewEnumName;
    int mOldIdx = 0;
    int mNewIdx = 0;
};

// TODO: Merge with above ?
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

    virtual QString VGetEnumName(s32 idx) const = 0;
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

    QString VGetEnumName(s32 idx) const override
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
