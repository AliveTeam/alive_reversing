#include "EditorGraphicsScene.hpp"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"
#include "CameraGraphicsItem.hpp"
#include "EditorTab.hpp"
#include "Model.hpp"
#include <QUndoCommand>
#include "DeleteItemsCommand.hpp"

EditorGraphicsScene::EditorGraphicsScene(EditorTab* pTab)
    : mTab(pTab)
{
    ToggleGrid();
}

QList<ResizeableRectItem*> EditorGraphicsScene::MapObjectsForCamera(CameraGraphicsItem* pCameraGraphicsItem)
{
    /*
    const auto& modelMapObjects = pCameraGraphicsItem->GetCamera()->mMapObjects;

    QList<ResizeableRectItem*> graphicsItemMapObjects;
    QList<QGraphicsItem*> allItems = items();
    for (QGraphicsItem* item : allItems)
    {
        ResizeableRectItem* pCastedGraphicsItemMapObject = qgraphicsitem_cast<ResizeableRectItem*>(item);
        if (pCastedGraphicsItemMapObject)
        {
            for (auto& mapModelObj : modelMapObjects)
            {
                if (mapModelObj.get() == pCastedGraphicsItemMapObject->GetMapObject())
                {
                    graphicsItemMapObjects.append(pCastedGraphicsItemMapObject);
                    break;
                }
            }
        }
    }
    return graphicsItemMapObjects;
    */
    return {};
}

void EditorGraphicsScene::UpdateSceneRect()
{
    /*
    const int kXMargin = 100;
    const int kYMargin = 100;
    const auto& mapInfo = mTab->GetModel().GetMapInfo();
    setSceneRect(-kXMargin, -kYMargin, (mapInfo.mXSize * mapInfo.mXGridSize) + (kXMargin * 2), (mapInfo.mYSize * mapInfo.mYGridSize) + (kYMargin * 2));
    */
}

CameraGraphicsItem* EditorGraphicsScene::CameraAt(int x, int y)
{
    /*
    QList<QGraphicsItem*> allItems = items();
    for (QGraphicsItem* item : allItems)
    {
        CameraGraphicsItem* pCameraItem = qgraphicsitem_cast<CameraGraphicsItem*>(item);
        if (pCameraItem)
        {
            if (pCameraItem->GetCamera()->mX == x && pCameraItem->GetCamera()->mY == y)
            {
                return pCameraItem;
            }
        }
    }
    */
    return nullptr;
}

TransparencySettings& EditorGraphicsScene::GetTransparencySettings()
{
    return mTransparencySettings;
}

void EditorGraphicsScene::SyncTransparencySettings()
{
    QList<QGraphicsItem*> objs = items();
    for (auto& obj : objs)
    {
        auto pCameraGraphicsItem = qgraphicsitem_cast<CameraGraphicsItem*>(obj);
        if (pCameraGraphicsItem)
        {
            IGraphicsItem::SetTransparency(pCameraGraphicsItem, mTransparencySettings.CameraTransparency());
        }
        else
        {
            auto pResizeableArrowItem = qgraphicsitem_cast<ResizeableArrowItem*>(obj);
            if (pResizeableArrowItem)
            {
                pResizeableArrowItem->SetTransparency(pResizeableArrowItem, mTransparencySettings.CollisionTransparency());
            }
            else
            {
                auto pResizeableRectItem = qgraphicsitem_cast<ResizeableRectItem*>(obj);
                if (pResizeableRectItem)
                {
                    pResizeableRectItem->SetTransparency(pResizeableRectItem, mTransparencySettings.MapObjectTransparency());
                }
            }
        }
    }
}

void EditorGraphicsScene::CreateBackgroundBrush()
{
    int gridSizeX = 25;
    int gridSizeY = 20;

    // Paint a single grid background onto a pixmap.
    QImage singleGrid(gridSizeX, gridSizeY, QImage::Format_RGB32);
    singleGrid.fill(QColor(50, 50, 50));
    {
        QPainter painter(&singleGrid);

        // Lighter background
        /*painter.setPen(QPen(QColor(140, 140, 140)));

        // Draw grid mid lines
        qreal midx = gridSizeX / 2;
        qreal midy = gridSizeY / 2;
        painter.drawLine(0, midy, gridSizeX, midy);
        painter.drawLine(midx, 0, midx, gridSizeY);*/

        // Darker foreground
        painter.setPen(QPen(QColor(80, 80, 80)));

        // Draw main grid lines
        painter.drawRect(0, 0, gridSizeX, gridSizeY);

        painter.end();
    }

    // Use this as the background brush which will be tiled and scale
    // properly when the scene is zoomed. And also prevents rendering issues compared to
    // the overriding drawBackground() method.
    QBrush brushBackground(singleGrid);
    setBackgroundBrush(brushBackground);
}

void EditorGraphicsScene::ToggleGrid()
{
    mGridEnabled = !mGridEnabled;
    if (mGridEnabled)
    {
        CreateBackgroundBrush();
    }
    else
    {
        QBrush b;
        setBackgroundBrush(b);
    }
}

void EditorGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
    {
        qDebug() << "Ignore non left click";
        pEvent->ignore();
        return;
    }

    if (pEvent->button() == Qt::LeftButton)
    {
        mLeftButtonDown = true;

        qDebug() << "left press";

        // Record what we had before a click
        mOldSelection = selectedItems();

        // Do the click
        QGraphicsScene::mousePressEvent(pEvent);

        if (mOldSelection != selectedItems())
        {
            qDebug() << "left press selection changed";

            // A single item just got selected by being clicked on
            emit SelectionChanged(mOldSelection, selectedItems());
            mOldSelection = selectedItems();
        }

        // Save the locations of what is selected after click
        mOldPositions.Save(mOldSelection, mTab->GetModel(), false);
    }
    else
    {
        qDebug() << "mouse press";
        QGraphicsScene::mousePressEvent(pEvent);
    }
}

void EditorGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsScene::mouseMoveEvent(pEvent);
}

void EditorGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
    // Handle the button up
    QGraphicsScene::mouseReleaseEvent(pEvent);

    if (pEvent->button() == Qt::LeftButton)
    {
        qDebug() << "left release";

        // Find out what is selected
        QList<QGraphicsItem*> currentSelection = selectedItems();

        if (mOldSelection != currentSelection)
        {
            // Between mouse up/down the selection changed
            qDebug() << "left release selection changed";
            emit SelectionChanged(mOldSelection, currentSelection);
        }

        if (mOldPositions.Count() > 0)
        {
            // Get the position of where the selected items are now
            ItemPositionData newPositions;
            newPositions.Save(currentSelection, mTab->GetModel(), true);

            if (mOldPositions != newPositions)
            {
                qDebug() << "left release positions changed";

                // They've moved since mouse down
                emit ItemsMoved(mOldPositions, newPositions);
            }
        }

        mLeftButtonDown = false;
    }
}

void EditorGraphicsScene::keyPressEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_Delete)
    {
        QList<QGraphicsItem*> selected = selectedItems();
        if (!selected.isEmpty())
        {
            mTab->AddCommand(new DeleteItemsCommand(mTab, false, selected));
        }
    }
    else
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}


Camera* CalcContainingCamera(ResizeableRectItem* pItem, Model& model)
{
    /*
    QPoint midPoint = pItem->CurrentRect().center().toPoint();

    int camX = midPoint.x() / model.GetMapInfo().mXGridSize;
    if (camX < 0)
    {
        camX = 0;
    }

    if (camX >= model.GetMapInfo().mXSize)
    {
        camX = model.GetMapInfo().mXSize - 1;
    }

    int camY = midPoint.y() / model.GetMapInfo().mYGridSize;
    if (camY < 0)
    {
        camY = 0;
    }

    if (camY >= model.GetMapInfo().mYSize)
    {
        camY = model.GetMapInfo().mYSize - 1;
    }

    return model.CameraAt(camX, camY);
    */
    return nullptr;
}


void ItemPositionData::Save(QList<QGraphicsItem*>& items, Model& model, bool recalculateParentCamera)
{
    mRects.clear();
    mLines.clear();

    for (auto& item : items)
    {
        ResizeableRectItem* pRect = qgraphicsitem_cast<ResizeableRectItem*>(item);
        if (pRect)
        {
            AddRect(pRect, model, recalculateParentCamera);
        }
        else
        {
            ResizeableArrowItem* pArrow = qgraphicsitem_cast<ResizeableArrowItem*>(item);
            if (pArrow)
            {
                AddLine(pArrow);
            }
        }
    }
}

void ItemPositionData::Restore(Model& model)
{
    for (auto& [rect, pos] : mRects)
    {
        rect->SetRect(pos.rect);
        //model.SwapContainingCamera(rect->GetMapObject(), pos.containingCamera);
    }

    for (auto& [line, pos] : mLines)
    {
        line->RestoreLine(pos.line);
        line->setX(pos.x);
        line->setY(pos.y);
    }
}

void ItemPositionData::AddRect(ResizeableRectItem* pItem, Model& model, bool recalculateParentCamera)
{
    Camera* pContainingCamera = nullptr;
    MapObject* pMapObject = pItem->GetMapObject();

    if (recalculateParentCamera)
    {
        pContainingCamera = CalcContainingCamera(pItem, model);

       //model.SwapContainingCamera(pMapObject, pContainingCamera);
    }
    else
    {
      //  pContainingCamera = model.GetContainingCamera(pMapObject);
    }

    mRects[pItem] = { pItem->CurrentRect(), pContainingCamera };
}

void ItemPositionData::AddLine(ResizeableArrowItem* pItem)
{
    mLines[pItem] = { pItem->x(), pItem->y(), pItem->SaveLine() };
}
