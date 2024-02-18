#pragma once

#include <QGraphicsLineItem>
#include <QGraphicsView>
#include "IGraphicsItem.hpp"

class CollisionObject;
class ISyncPropertiesToTree;
class SnapSettings;
class IPointSnapper;

class ResizeableArrowItem final : public IGraphicsItem, public QGraphicsLineItem
{
public:
    ResizeableArrowItem(QGraphicsView* pView, CollisionObject* pLine, ISyncPropertiesToTree& propSyncer, int transparency, SnapSettings& snapSettings, IPointSnapper& snapper);
    enum { Type = UserType + 2 };
    int type() const override { return Type; }
    QLineF SaveLine() const;
    void RestoreLine(const QLineF& line);
    CollisionObject* GetCollisionItem() const
    {
        return mLine;
    }

    void SyncInternalObject() override
    {
        SyncToCollisionItem();
    }

    void Visit(IReflector& f) override;

protected:
    void hoverLeaveEvent( QGraphicsSceneHoverEvent* aEvent ) override;
    void hoverMoveEvent( QGraphicsSceneHoverEvent* aEvent ) override;
    void mousePressEvent( QGraphicsSceneMouseEvent* aEvent ) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* aEvent ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* aEvent ) override;
    void paint( QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget = nullptr ) override;
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue) override;
private:
    void Init();
    void CalcWhichEndOfLineClicked( QPointF aPos, Qt::KeyboardModifiers aMods );
    void SetViewCursor(Qt::CursorShape cursor);
    void SyncToCollisionItem();
    void PosOrLineChanged();
private:
    // For knowing which end to anchor line if required.
    enum eLinePoints
    {
        eLinePoints_None,
        eLinePoints_P1,
        eLinePoints_P2
    };
    eLinePoints m_endOfLineClicked = eLinePoints_None;
    QPointF m_AnchorPoint;
    QLineF m_MouseDownLine;
    bool m_MouseIsDown = false;
    QGraphicsView* mView = nullptr;
    CollisionObject* mLine = nullptr;
    ISyncPropertiesToTree& mPropSyncer;

    SnapSettings& mSnapSettings;
    IPointSnapper& mSnapper;
};
