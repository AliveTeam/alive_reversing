#include "ResizeableArrowItem.hpp"
#include <QCursor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "Model.hpp"
#include "PropertyTreeWidget.hpp"
#include "SnapSettings.hpp"

ResizeableArrowItem::ResizeableArrowItem(QGraphicsView* pView, Model::CollisionObject* pLine, ISyncPropertiesToTree& propSyncer, int transparency, SnapSettings& snapSettings, IPointSnapper& snapper)
    : QGraphicsLineItem(pLine->X2(), pLine->Y2(), pLine->X1(), pLine->Y1())
    , mView(pView)
    , mLine(pLine)
    , mPropSyncer(propSyncer)
    , mSnapSettings(snapSettings)
    , mSnapper(snapper)
{
    Init();
    setZValue(2.0);
    SetTransparency(this, transparency);
}

void ResizeableArrowItem::hoverLeaveEvent( QGraphicsSceneHoverEvent* aEvent )
{
    SetViewCursor( Qt::ArrowCursor );
    QGraphicsItem::hoverLeaveEvent( aEvent );
}

void ResizeableArrowItem::hoverMoveEvent( QGraphicsSceneHoverEvent* aEvent )
{
    if ( !m_MouseIsDown )
    {
        m_MouseDownLine = line();
        CalcWhichEndOfLineClicked( aEvent->pos(), aEvent->modifiers() );
        if ( m_endOfLineClicked == eLinePoints_None )
        {
            SetViewCursor( Qt::OpenHandCursor );
        }
        else
        {
            SetViewCursor( Qt::CrossCursor );
        }
        return;
    }
    QGraphicsItem::hoverMoveEvent( aEvent );
}

void ResizeableArrowItem::mousePressEvent( QGraphicsSceneMouseEvent* aEvent )
{
    if ( aEvent->button() == Qt::LeftButton )
    {
        m_MouseIsDown = true;
        SetViewCursor( Qt::ClosedHandCursor );
        CalcWhichEndOfLineClicked( aEvent->pos(), aEvent->modifiers() );
    }
    QGraphicsLineItem::mousePressEvent( aEvent );
}

void ResizeableArrowItem::mouseMoveEvent( QGraphicsSceneMouseEvent* aEvent )
{
    if ( m_endOfLineClicked == eLinePoints_None )
    {
        QGraphicsLineItem::mouseMoveEvent(aEvent);

        QPointF tl = pos();
        setPos(QPointF());

        QLineF tmp = m_MouseDownLine;
        tmp.translate(tl);
        setLine(tmp);

        PosOrLineChanged();
        return;
    }

    QLineF newLine = m_endOfLineClicked == eLinePoints_P1  ? QLineF( aEvent->pos(), m_AnchorPoint ) : QLineF( m_AnchorPoint, aEvent->pos() );
    const auto kMinLineLength = 15;
    if ( newLine.length() <= kMinLineLength )
    {
        if ( m_endOfLineClicked == eLinePoints_P2 )
        {
            newLine.setLength( kMinLineLength );
        }
        else
        {
            newLine = QLineF( newLine.p2(), newLine.p1() );
            newLine.setLength( kMinLineLength );
            newLine = QLineF( newLine.p2(), newLine.p1() );
        }
    }

    if (m_endOfLineClicked == eLinePoints_P1)
    {
        QPoint tmp = newLine.p1().toPoint();
        tmp.setX(mSnapper.SnapX(mSnapSettings.CollisionSnapping().mSnapX, tmp.x()));
        tmp.setY(mSnapper.SnapY(mSnapSettings.CollisionSnapping().mSnapY, tmp.y()));
        newLine.setP1(tmp);
    }
    else
    {
        QPoint tmp = newLine.p2().toPoint();
        tmp.setX(mSnapper.SnapX(mSnapSettings.CollisionSnapping().mSnapX, tmp.x()));
        tmp.setY(mSnapper.SnapY(mSnapSettings.CollisionSnapping().mSnapY, tmp.y()));
        newLine.setP2(tmp);
    }

    setLine( newLine );
    PosOrLineChanged();
}

void ResizeableArrowItem::mouseReleaseEvent( QGraphicsSceneMouseEvent* aEvent )
{
    if ( aEvent->button() == Qt::LeftButton )
    {
        m_MouseIsDown = false;
    }
    QGraphicsLineItem::mouseReleaseEvent( aEvent );
}

void ResizeableArrowItem::paint( QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget /*= nullptr*/ )
{
    Q_UNUSED( aWidget );

    // Only draw what is required
    aPainter->setClipRect( aOption->exposedRect );
    switch (mLine->mLine.mLineType)
    {
        case eLineTypes::eArt_9:
            aPainter->setBrush(QColor(100, 100, 100, 255));
            break;

        case eLineTypes::eBackgroundFloor_4:
        case eLineTypes::eBackgroundWallLeft_5:
        case eLineTypes::eBackgroundWallRight_6:
        case eLineTypes::eBackgroundCeiling_7:
            aPainter->setBrush(QColor(150, 150, 75, 255));
            break;

        case eLineTypes::eTrackLine_8:
            aPainter->setBrush(QColor(0, 215, 215, 255));
            break;

        case eLineTypes::eBulletWall_10:
            aPainter->setBrush(QColor(255, 70, 70, 255));
            break;

        case eLineTypes::eMineCarFloor_11:
        case eLineTypes::eMineCarWall_12:
        case eLineTypes::eMineCarCeiling_13:
        case eLineTypes::eBackgroundMineCarFloor_14:
        case eLineTypes::eBackgroundMineCarWall_15:
        case eLineTypes::eBackgroundMineCarCeiling_16:
            aPainter->setBrush(QColor(190, 70, 255, 255));
            break;

        case eLineTypes::eFlyingObjectWall_17:
        case eLineTypes::eBackgroundFlyingObjectWall_18:
            aPainter->setBrush(QColor(30, 200, 15, 255));
            break;

        case eLineTypes::eFloor_0:
        case eLineTypes::eWallLeft_1:
        case eLineTypes::eWallRight_2:
        case eLineTypes::eCeiling_3:
        default:
            aPainter->setBrush(QColor(255, 255, 100, 255));
            break;
    }

    // Change the pen depending on selection
    if ( isSelected() )
    {
        aPainter->setPen( QPen ( Qt::red, 2, Qt::DashLine )  );
    }
    else
    {
        QPen p( Qt::black, 2, Qt::SolidLine );
        p.setJoinStyle( Qt::RoundJoin );
        aPainter->setPen( p );
    }

    // Use the painter path for rendering
    aPainter->drawPath( shape() );
}

QPainterPath ResizeableArrowItem::shape() const
{
    // Calc arrow head lines based on the angle of the current line
    QLineF cLine = line();

    const auto kArrowHeadLength = 8;
    const auto kArrowHeadAngle = 32;

    const qreal cLineAngle = cLine.angle();
    QLineF head1 = cLine;
    QLineF head2 = cLine;
    head1.setLength( kArrowHeadLength );
    head1.setAngle( cLineAngle+-kArrowHeadAngle );
    head2.setLength( kArrowHeadLength );
    head2.setAngle( cLineAngle+kArrowHeadAngle );

    // Create paths for each section of the arrow
    QPainterPath mainLine;
    mainLine.moveTo( cLine.p2() );
    mainLine.lineTo( cLine.p1() );


    QPainterPath headLine1;
    headLine1.moveTo( cLine.p1() );
    headLine1.lineTo( head1.p2() );

    QPainterPath headLine2;
    headLine2.moveTo( cLine.p1() );
    headLine2.lineTo( head2.p2() );

    QPainterPathStroker stroker;
    stroker.setWidth( 4 );

    // Join them together
    QPainterPath stroke = stroker.createStroke( mainLine );
    stroke.addPath( stroker.createStroke( headLine1 ) );
    stroke.addPath( stroker.createStroke( headLine2 ) );

    return stroke.simplified();
}

QRectF ResizeableArrowItem::boundingRect() const
{
    QPainterPath pPath = shape();

    QRectF bRect = pPath.controlPointRect();

    QRectF adjusted( bRect.x()-1, bRect.y()-1, bRect.width()+2, bRect.height()+2 );

    return adjusted;
}

QVariant ResizeableArrowItem::itemChange(GraphicsItemChange aChange, const QVariant& aValue)
{
    if (aChange == ItemPositionHasChanged)
    {
        PosOrLineChanged();
    }
    return QGraphicsLineItem::itemChange(aChange, aValue);
}

void ResizeableArrowItem::Init()
{
    setToolTip("Click and drag an edge of the line to resize it, or hold shift and click and drag to move the line");

    QPen p( Qt::black, 2, Qt::SolidLine );
    p.setJoinStyle( Qt::RoundJoin );
    setPen( p );

    setAcceptHoverEvents( true );

    SetViewCursor( Qt::CrossCursor );

    // Allow select and move.
    setFlags( ItemSendsScenePositionChanges | ItemSendsGeometryChanges | ItemIsMovable | ItemIsSelectable );

    prepareGeometryChange();

    m_endOfLineClicked = eLinePoints_None;
    m_MouseIsDown = false;

    // TODO: Use QPixmapCache instead
    setCacheMode( ItemCoordinateCache );
}

void ResizeableArrowItem::CalcWhichEndOfLineClicked( QPointF aPos, Qt::KeyboardModifiers aMods )
{
    if ( aMods == Qt::ShiftModifier )
    {
        m_endOfLineClicked = eLinePoints_None;
        return;
    }

    // Figure out which part of the line we clicked, either one of the ends or the middle!
    const QPointF mousePos = aPos;

    // Get the distance from each end of the line
    const QLineF p1Distance( mousePos, line().p1() );
    const QLineF p2Distance( mousePos, line().p2() );

    // Ensure the distance is near the end, if not then only allow moving rather than resizing.
    if ( line().length() > 10 )
    {
        const qreal tolerance = 10;
        const qreal shortestDistance = p1Distance.length() > p2Distance.length() ? p2Distance.length() : p1Distance.length();
        if ( shortestDistance > tolerance )
        {
            m_endOfLineClicked = eLinePoints_None;
            return;
        }
    }

    m_endOfLineClicked = p1Distance.length() > p2Distance.length()  ? eLinePoints_P2 : eLinePoints_P1;
    m_AnchorPoint = m_endOfLineClicked == eLinePoints_P2 ? line().p1() : line().p2();
}

void ResizeableArrowItem::SetViewCursor(Qt::CursorShape cursor)
{
    mView->setCursor(cursor);
}

QLineF ResizeableArrowItem::SaveLine() const
{
    return line();
}

void ResizeableArrowItem::RestoreLine(const QLineF& line)
{
    setLine(line);
    PosOrLineChanged();
}

void ResizeableArrowItem::SyncToCollisionItem()
{
    setLine(mLine->X2(), mLine->Y2(), mLine->X1(), mLine->Y1());
}

void ResizeableArrowItem::PosOrLineChanged()
{
    QLineF curLine = line();

    // Sync the model to the graphics item
    mLine->SetX1(static_cast<int>(curLine.x2()));
    mLine->SetY1(static_cast<int>(curLine.y2()));
    mLine->SetX2(static_cast<int>(curLine.x1()));
    mLine->SetY2(static_cast<int>(curLine.y1()));

    // Update the property tree view
    mPropSyncer.Sync(this);
}
