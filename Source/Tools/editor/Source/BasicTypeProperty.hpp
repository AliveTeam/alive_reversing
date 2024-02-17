#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>
#include <QtGlobal>

class QUndoStack;
class BigSpinBox;

enum class IntegerType
{
    Int_S16,
    Int_U16,
    Int_S32,
    Int_U32,
};

struct BasicTypePropertyChangeData
{
    BasicTypePropertyChangeData(MapObjectBase* mapObject, IntegerType intType, void* integerPtr, qint64 oldValue, qint64 newValue)
        : mMapObject(mapObject), mIntType(intType), mIntegerPtr(integerPtr), mOldValue(oldValue), mNewValue(newValue)
    {

    }

    MapObjectBase* mMapObject = nullptr;
    IntegerType mIntType = IntegerType::Int_S16;
    void* mIntegerPtr = nullptr;
    qint64 mOldValue = 0;
    qint64 mNewValue = 0;
};

// TODO: probably merge with BasicTypePropertyChangeData ??
struct LinkedBasicTypeProperty
{
    LinkedBasicTypeProperty(MapObjectBase* mapObject, const char* propertyName, IntegerType intType, void* pIntPtr, PropertyTreeWidget* pTreeWidget, IGraphicsItem* pGraphicsItem)
        : mMapObject(mapObject), mPropertyName(propertyName), mIntegerType(intType), mIntPtr(pIntPtr), mTreeWidget(pTreeWidget), mGraphicsItem(pGraphicsItem)
    {

    }

    MapObjectBase* mMapObject = nullptr;
    const char* mPropertyName = nullptr;
    IntegerType mIntegerType;
    void* mIntPtr = nullptr;
    PropertyTreeWidget* mTreeWidget = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
};

class ChangeBasicTypePropertyCommand final : public QUndoCommand
{
public:
    ChangeBasicTypePropertyCommand(LinkedBasicTypeProperty linkedProperty, BasicTypePropertyChangeData propertyData);

    void undo() override;

    void redo() override;

    int id() const override
    {
        return 1;
    }

    bool mergeWith(const QUndoCommand* command) override;

private:
    void UpdateText();
    LinkedBasicTypeProperty mLinkedProperty;
    BasicTypePropertyChangeData mPropertyData;
    qint64 mTimeStamp = 0;
};

class BasicTypeProperty final : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:

    BasicTypeProperty(IntegerType intType, void* pInteger, const char* pPropertyName, QUndoStack& undoStack, MapObjectBase* pMapObject, IGraphicsItem* pGraphicsItem);

    QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    const void* GetPropertyLookUpKey() const override
    {
        return mIntegerPtr;
    }

    void Refresh() override;

private:

    IntegerType mIntType = IntegerType::Int_S16;
    void* mIntegerPtr = nullptr;
    const char* mPropertyName = nullptr;
    QUndoStack& mUndoStack;
    MapObjectBase* mMapObject = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
    BigSpinBox* mSpinBox = nullptr;
    qint64 mOldValue = 0;
};
