#include "ClipBoard.hpp"
#include "EditorTab.hpp"
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"

PasteItemsCommand::PasteItemsCommand(EditorTab* pTab, ClipBoard& clipBoard)
    : mTab(pTab), mSelectionSaver(pTab)
{
    /*
    // Make another deep copy of the items and create graphics items for them
    mCollisions = clipBoard.CloneCollisions(nullptr);
    for (auto& obj : mCollisions)
    {
        // Fix collision line ids
        obj->mId = mTab->GetModel().NextCollisionId();
        mCollisionGraphicsObjects.emplace_back(mTab->MakeResizeableArrowItem(obj.get()));
    }

    auto clonedMapObjects = clipBoard.CloneMapObjects(nullptr);
    for (auto& obj : clonedMapObjects)
    {
        // Create the graphics item
        ResizeableRectItem* mapObjectGraphicsItem = mTab->MakeResizeableRectItem(obj.get());

        // Ensure map object is in the correct camera
        Camera* containingCamera = CalcContainingCamera(mapObjectGraphicsItem, mTab->GetModel());

        PastedMapObject pastedMapObject;
        pastedMapObject.mContainingCamera = containingCamera;
        pastedMapObject.mPastedMapObject = std::move(obj);

        // Keep track of the graphics item and the camera it lives in
        mMapObjects.emplace_back(std::move(pastedMapObject));
        mMapGraphicsObjects.emplace_back(mapObjectGraphicsItem);
    }

    setText("Paste " + QString::number(mCollisions.size() + mMapObjects.size()) +  " item(s)");
    */
}

PasteItemsCommand::~PasteItemsCommand()
{
    // Delete copies if not added
    if (!mPasted)
    {
        for (auto& obj : mMapGraphicsObjects)
        {
            delete obj;
        }

        for (auto& obj : mCollisionGraphicsObjects)
        {
            delete obj;
        }
    }
}

void PasteItemsCommand::redo()
{
    /*
    // Add to scene
    for (auto& obj : mMapGraphicsObjects)
    {
        mTab->GetScene().addItem(obj);
    }

    // Add to model
    for (auto& obj : mMapObjects)
    {
        obj.mContainingCamera->mMapObjects.emplace_back(std::move(obj.mPastedMapObject));
    }
    mMapObjects.clear();

    // Add to scene
    for (auto& obj : mCollisionGraphicsObjects)
    {
        mTab->GetScene().addItem(obj);
    }

    // Add to model
    for (auto& obj : mCollisions)
    {
        mTab->GetModel().CollisionItems().emplace_back(std::move(obj));
    }
    mCollisions.clear();

    mTab->GetScene().clearSelection();

    // Select what was pasted
    for (auto& obj : mMapGraphicsObjects)
    {
        obj->setSelected(true);
    }

    for (auto& obj : mCollisionGraphicsObjects)
    {
        obj->setSelected(true);
    }

    mPasted = true;

    mSelectionSaver.redo();
    */
}

void PasteItemsCommand::undo()
{
    /*
    for (auto& obj : mMapGraphicsObjects)
    {
        // Remove from model
        PastedMapObject pastedMapObject;
        pastedMapObject.mContainingCamera = mTab->GetModel().GetContainingCamera(obj->GetMapObject());
        pastedMapObject.mPastedMapObject = mTab->GetModel().TakeFromContainingCamera(obj->GetMapObject());
        
        mMapObjects.emplace_back(std::move(pastedMapObject));

        // Remove from scene
        mTab->GetScene().removeItem(obj);
    }

    for (auto& obj : mCollisionGraphicsObjects)
    {
        // Remove from model
        mCollisions.emplace_back(mTab->GetModel().RemoveCollisionItem(obj->GetCollisionItem()));

        // Remove from scene
        mTab->GetScene().removeItem(obj);
    }

    mPasted = false;

    mSelectionSaver.undo();
    */
}

void ClipBoard::Set(const QList<QGraphicsItem*>& items, Model& model)
{
    /*
    if (items.isEmpty())
    {
        return;
    }

    mSourceGame = model.GetMapInfo().mGame;

    mMapObjects.clear();
    mCollisions.clear();

    // Deep copy the items for pasting
    for (int i = 0; i < items.count(); i++)
    {
        QGraphicsItem* obj = items.at(i);
        auto pResizeableRectItem = qgraphicsitem_cast<ResizeableRectItem*>(obj);
        if (pResizeableRectItem)
        {
            mMapObjects.emplace_back(std::make_unique<MapObject>(*pResizeableRectItem->GetMapObject()));
        }
        else
        {
            auto pResizeableArrowItem = qgraphicsitem_cast<ResizeableArrowItem*>(obj);
            if (pResizeableArrowItem)
            {
               mCollisions.emplace_back(std::make_unique<CollisionObject>(0, *pResizeableArrowItem->GetCollisionItem()));
            }
        }
    }
    */
}

bool ClipBoard::IsEmpty() const
{
    //return mMapObjects.empty() && mCollisions.empty();
    return true;
}

const std::string& ClipBoard::SourceGame() const
{
    return mSourceGame;
}

/*/
std::vector<UP_MapObject> ClipBoard::CloneMapObjects(QPoint* pos) const
{
    QPoint offset(50, 50);
    if (pos)
    {
        offset = *pos;
    }

    // TODO: Position set or offsetting via pos
    std::vector<UP_MapObject> r;
    for (auto& obj : mMapObjects)
    {
        auto copy = std::make_unique<MapObject>(*obj);
        copy->SetXPos(copy->XPos() + offset.x());
        copy->SetYPos(copy->YPos() + offset.y());
        r.emplace_back(std::move(copy));
    }
    return r;
}

std::vector<UP_CollisionObject> ClipBoard::CloneCollisions(QPoint* pos) const
{
    // TODO: Position set or offsetting via pos

    std::vector<UP_CollisionObject> r;
    for (auto& obj : mCollisions)
    {
        r.emplace_back(std::make_unique<CollisionObject>(0, *obj));
    }
    return r;
}
*/
