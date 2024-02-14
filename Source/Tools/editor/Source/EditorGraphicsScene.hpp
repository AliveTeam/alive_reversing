#pragma once

#include <QGraphicsScene>
#include <QKeyEvent>
#include <map>
#include "Model.hpp"

class ResizeableArrowItem;
class ResizeableRectItem;
class CameraGraphicsItem;
class EditorTab;

class ItemPositionData final
{
public:
    struct RectPos final
    {
        QRectF rect;
        Model::Camera* containingCamera = nullptr;

        bool operator == (const RectPos& rhs) const
        {
            return rect == rhs.rect && containingCamera == rhs.containingCamera;
        }
    };

    struct LinePos final
    {
        qreal x = 0;
        qreal y = 0;
        QLineF line;

        bool operator == (const LinePos& rhs) const
        {
            return x == rhs.x && y == rhs.y && line == rhs.line;
        }
    };

    void Save(QList<QGraphicsItem*>& items, Model& model, bool recalculateParentCamera);
    void Restore(Model& model);

    bool operator == (const ItemPositionData& rhs) const
    {
        if (mRects != rhs.mRects)
        {
            return false;
        }

        if (mLines != rhs.mLines)
        {
            return false;
        }

        return true;
    }

    bool operator != (const ItemPositionData& rhs) const
    {
        return !(*this == rhs);
    }

    size_t Count() const
    {
        return mRects.size() + mLines.size();
    }

    const RectPos* FirstRectPos() const
    {
        if (mRects.empty())
        {
            return nullptr;
        }
        return &mRects.begin()->second;
    }

    const LinePos* FirstLinePos() const
    {
        if (mLines.empty())
        {
            return nullptr;
        }
        return &mLines.begin()->second;
    }
private:
    void AddRect(ResizeableRectItem* pItem, Model& model, bool recalculateParentCamera);

    void AddLine(ResizeableArrowItem* pItem);

    std::map<ResizeableRectItem*, RectPos> mRects;
    std::map<ResizeableArrowItem*, LinePos> mLines;
};

class TransparencySettings final
{
public:
    int CameraTransparency() const
    {
        return mCameraTransparency;
    }

    int CollisionTransparency() const
    {
        return mCollisionTransparency;
    }

    int MapObjectTransparency() const
    {
        return mMapObjectTransparency;
    }

    void SetCameraTransparency(int value)
    {
        mCameraTransparency = value;
    }

    void SetCollisionTransparency(int value)
    {
        mCollisionTransparency = value;
    }

    void SetMapObjectTransparency(int value)
    {
        mMapObjectTransparency = value;
    }

private:
    int mCameraTransparency = 70;
    int mCollisionTransparency = 90;
    int mMapObjectTransparency = 60;
};

Model::Camera* CalcContainingCamera(ResizeableRectItem* pItem, Model& model);

class EditorGraphicsScene final : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EditorGraphicsScene(EditorTab* pTab);

    QList<ResizeableRectItem*> MapObjectsForCamera(CameraGraphicsItem* pCameraGraphicsItem);

    void UpdateSceneRect();

    CameraGraphicsItem* CameraAt(int x, int y);

    TransparencySettings& GetTransparencySettings();

    void SyncTransparencySettings();
    void ToggleGrid();


signals:
    void SelectionChanged(QList<QGraphicsItem*> oldItems, QList<QGraphicsItem*> newItems);
    void ItemsMoved(ItemPositionData oldPositions, ItemPositionData newPositions);
private:
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

    void keyPressEvent(QKeyEvent* keyEvent) override;

    void CreateBackgroundBrush();

private:
    EditorTab* mTab = nullptr;
    QList<QGraphicsItem*> mOldSelection;
    ItemPositionData mOldPositions;
    bool mLeftButtonDown = false;
    TransparencySettings mTransparencySettings;
    bool mGridEnabled = false;
};
