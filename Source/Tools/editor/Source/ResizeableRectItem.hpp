#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include "IGraphicsItem.hpp"

class MapObjectBase;
class ISyncPropertiesToTree;
class SnapSettings;
class IPointSnapper;

class ResizeableRectItem final : public IGraphicsItem, public QGraphicsItem
{
public:
    ResizeableRectItem(QGraphicsView* pView, MapObjectBase* pMapObject, ISyncPropertiesToTree& propSyncer, int transparency, SnapSettings& snapSettings, IPointSnapper& snapper);
    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    QRectF CurrentRect() const;
    void SetRect(const QRectF& rect);
    MapObjectBase* GetMapObject() const { return mMapObject; }
 
    void SyncInternalObject() override
    {
        SyncFromMapObject();
    }

    void Visit(IReflector& f) override;

private:  // From QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* aEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* aEvent) override;
    void paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget = nullptr) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* aEvent) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;
    QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue) override;
    QRectF boundingRect() const override;
private:
    enum eResize
    {
        eResize_None,
        eResize_TopLeftCorner,
        eResize_TopRightCorner,
        eResize_BottomLeftCorner,
        eResize_BottomRightCorner,
        eResize_Top,
        eResize_Left,
        eResize_Right,
        eResize_Bottom
    };
    void Init();
    void UpdateIcon();
    eResize getResizeLocation( QPointF aPos, QRectF aRect );
    bool IsNear( qreal xP1, qreal xP2 );
    void onResize( QPointF aPos );
    void SetViewCursor(Qt::CursorShape cursor);
    qreal CalcZPos() const;
    void SyncFromMapObject();
    void PosOrRectChanged();
    
    void setWidth(int width)
    {
        if (mWidth != width)
        {
            prepareGeometryChange();
            mWidth = width;
        }
    }

    void setHeight(int height)
    {
        if (mHeight != height)
        {
            prepareGeometryChange();
            mHeight = height;
        }
    }

    void SyncToMapObject();
    QString ObjectNameInTitleCase();

private:
    eResize m_ResizeMode = eResize_None;
    static const quint32 kMinRectSize;
    QPixmap m_Pixmap;
    QGraphicsView* mView = nullptr;
    MapObjectBase* mMapObject = nullptr;
    ISyncPropertiesToTree& mPropSyncer;
    int mWidth = 0;
    int mHeight = 0;

    SnapSettings& mSnapSettings;
    IPointSnapper& mPointSnapper;

    QString mNoIconObjectName = "";
    QString mIconPath;
};
