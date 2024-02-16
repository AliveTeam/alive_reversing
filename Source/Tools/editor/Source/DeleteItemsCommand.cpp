#include "DeleteItemsCommand.hpp"
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"
#include "EditorTab.hpp"
#include "EditorGraphicsScene.hpp"

DeleteItemsCommand::DeleteItemsCommand(EditorTab* pTab, bool isCut, QList<QGraphicsItem*> graphicsItemsToDelete)
    : mTab(pTab), mGraphicsItemsToDelete(graphicsItemsToDelete)
{
    if (isCut)
    {
        setText("Cut " + QString::number(mGraphicsItemsToDelete.count()) + " item(s)");
    }
    else
    {
        setText("Delete " + QString::number(mGraphicsItemsToDelete.count()) + " item(s)");
    }
}

DeleteItemsCommand::~DeleteItemsCommand()
{
    if (!mAdded)
    {
        // Delete un-owned graphics items
        for (auto& item : mGraphicsItemsToDelete)
        {
            delete item;
        }
    }
}

void DeleteItemsCommand::undo()
{
    // add back to scene
    for (auto& item : mGraphicsItemsToDelete)
    {
        mTab->GetScene().addItem(item);
    }

    // add back to model
    for (auto& item : mRemovedCollisions)
    {
        mTab->GetModel().CollisionItems().emplace_back(std::move(item));
    }
    mRemovedCollisions.clear();

    // add back to model
    for (auto& item : mRemovedMapObjects)
    {
        item.mContainingCamera->mMapObjects.push_back(std::move(item.mRemovedMapObject));
    }

    mRemovedMapObjects.clear();

    mAdded = true;

    // Select whatever was selected before we deleted anything (which IS the stuff we deleted)
    for (auto& item : mGraphicsItemsToDelete)
    {
        item->setSelected(true);
    }

    mTab->GetScene().update();
    mTab->SyncPropertyEditor();
}

void DeleteItemsCommand::redo()
{
    // remove from scene
    for (auto& item : mGraphicsItemsToDelete)
    {
        mTab->GetScene().removeItem(item);

        ResizeableArrowItem* pArrow = qgraphicsitem_cast<ResizeableArrowItem*>(item);
        if (pArrow)
        {
            mRemovedCollisions.emplace_back(mTab->GetModel().RemoveCollisionItem(pArrow->GetCollisionItem()));
        }

        ResizeableRectItem* pRect = qgraphicsitem_cast<ResizeableRectItem*>(item);
        if (pRect)
        {
            // todo: prevent double cam look up
            auto pCam = mTab->GetModel().GetContainingCamera(pRect->GetMapObject());
            mRemovedMapObjects.emplace_back(DeletedMapObject{ mTab->GetModel().TakeFromContainingCamera(pRect->GetMapObject()), pCam });
        }
    }

    mAdded = false;

    // Select nothing after deleting the selection
    mTab->GetScene().clearSelection();
    mTab->GetScene().update();
    mTab->SyncPropertyEditor();
}
