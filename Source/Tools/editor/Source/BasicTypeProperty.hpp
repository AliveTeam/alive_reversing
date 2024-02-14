#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>

class QUndoStack;
class BigSpinBox;

struct BasicTypePropertyChangeData
{
    BasicTypePropertyChangeData(MapObjectBase* mapObject,  u32 propertyIdx, int oldValue, int newValue)
        : mMapObject(mapObject), mPropertyIdx(propertyIdx), mOldValue(oldValue), mNewValue(newValue)
    {

    }

    MapObjectBase* mMapObject = nullptr;
    u32 mPropertyIdx =0;
    int mOldValue = 0;
    int mNewValue = 0;
};

class ChangeBasicTypePropertyCommand final : public QUndoCommand
{
public:
    ChangeBasicTypePropertyCommand(LinkedProperty linkedProperty, BasicTypePropertyChangeData propertyData);

    void undo() override;

    void redo() override;

    int id() const override
    {
        return 1;
    }

    bool mergeWith(const QUndoCommand* command) override;

private:
    void UpdateText();
    LinkedProperty mLinkedProperty;
    BasicTypePropertyChangeData mPropertyData;
    qint64 mTimeStamp = 0;
};

class BasicTypeProperty final : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    BasicTypeProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, MapObjectBase* pMapObject, u32 propertyIdx, IGraphicsItem* pGraphicsItem);

    QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    const void* GetPropertyLookUpKey() const override
    {
        return mMapObject->PropertyPtr(mPropertyIdx);
    }

    void Refresh() override;

private:
    MapObjectBase* mMapObject = nullptr;
    u32 mPropertyIdx =0;
    QUndoStack& mUndoStack;
    IGraphicsItem* mGraphicsItem = nullptr;
    int mOldValue = 0;
    BigSpinBox* mSpinBox = nullptr;
};
