#include "ResizeableRectItem.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QPainter>
#include <QTimer>
#include <QPointF>
#include <QCursor>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmapCache>
#include "Model.hpp"
#include "PropertyTreeWidget.hpp"
#include "SnapSettings.hpp"

const quint32 ResizeableRectItem::kMinRectSize = 10;

ResizeableRectItem::ResizeableRectItem(QGraphicsView* pView, MapObjectBase* pMapObject, ISyncPropertiesToTree& propSyncer, int transparency, SnapSettings& snapSettings, IPointSnapper& snapper)
      : mView(pView), mMapObject(pMapObject), mPropSyncer(propSyncer), mSnapSettings(snapSettings), mPointSnapper(snapper)
{
    SyncFromMapObject();

    Init();
    setZValue(3.0 + CalcZPos());

    SetTransparency(this, transparency);
}

// TODO: Re-calc on new w/h
qreal ResizeableRectItem::CalcZPos() const
{
    // Why isn't area == 1 ?
    float area = ((float)mWidth * (float)mHeight) / 4294836225.0f;
    float percentArea = area * 100.0f;

    qreal zpos = 999999.0f - (percentArea*1000.0f);
    // Negative zpos underflows and breaks resize/selection.
    if (zpos < 0)
    {
        zpos = 0;
    }
    return zpos;
}

void ResizeableRectItem::mousePressEvent( QGraphicsSceneMouseEvent* aEvent )
{
    if (aEvent->button() == Qt::LeftButton && QGraphicsItem::isSelected())
    {
        m_ResizeMode = getResizeLocation(aEvent->pos(), boundingRect());
        if (m_ResizeMode == eResize_None)
        {
            SetViewCursor(Qt::ClosedHandCursor);
        }
    }
    QGraphicsItem::mousePressEvent( aEvent );
}

void ResizeableRectItem::mouseMoveEvent( QGraphicsSceneMouseEvent* aEvent )
{
    if ( m_ResizeMode != eResize_None )
    {
        onResize(mView->mapToParent(aEvent->scenePos().toPoint()));
        return;
    }
    QGraphicsItem::mouseMoveEvent( aEvent );

    QRectF curRect = CurrentRect();
    
    const auto w = curRect.width();
    const auto h = curRect.height();
    curRect.setX(mPointSnapper.SnapX(mSnapSettings.MapObjectSnapping().mSnapX, curRect.x()));
    curRect.setWidth(w);

    curRect.setY(mPointSnapper.SnapY(mSnapSettings.MapObjectSnapping().mSnapY, curRect.y()));
    curRect.setHeight(h);

    curRect = curRect.normalized();
    SetRect(curRect);
}

void ResizeableRectItem::mouseReleaseEvent( QGraphicsSceneMouseEvent* aEvent )
{
    if ( aEvent->button() == Qt::LeftButton )
    {
        m_ResizeMode = eResize_None;
    }
    QGraphicsItem::mouseReleaseEvent( aEvent );
}

void ResizeableRectItem::paint( QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget)
{
    QRectF cRect(0, 0, mWidth, mHeight);

    if ( isSelected() )
    {
        aPainter->setPen( QPen ( Qt::red, 2, Qt::DashLine ) );
    }
    else
    {
        // Draw normal rect
        aPainter->setPen( QPen( Qt::black, 2, Qt::SolidLine ) );
    }
    
    if ( m_Pixmap.isNull() )
    {
        aPainter->setBrush( Qt::darkGray );
    }
    else
    {
       // aPainter->setBrush( QBrush() );
        aPainter->drawPixmap( cRect.x(), cRect.y(), cRect.width(), cRect.height(), m_Pixmap );
    }

    // Draw the rect outline.
    aPainter->drawRect(cRect);

    // Draw the object name on the rect if no image is provided
    if (m_Pixmap.isNull())
    {
        // TODO: reactor this to work in some sane way
        QString objectName;
        if (mMapObject->mBaseTlv->mTlvType == ReliveTypes::eTimedMine )
        {
            objectName = "timed mine";
        }
        else
        {
            objectName = "unknown";
        }

        for (int sizeCandidate = 8; sizeCandidate > 1; sizeCandidate--)
        {
            QFont f = aPainter->font();
            f.setPointSize(sizeCandidate);
            aPainter->setFont(f);
            QFontMetricsF fm(f);
            const auto textRect = fm.boundingRect(cRect, Qt::AlignCenter | Qt::TextWrapAnywhere, objectName);

            if (textRect.width() < cRect.width() &&
                textRect.height() < cRect.height())
            {
                break;
            }
        }
        aPainter->drawText(cRect, Qt::AlignCenter | Qt::TextWrapAnywhere, objectName);
    }
}

QRectF ResizeableRectItem::boundingRect() const
{
    qreal penWidth = 2;
    return QRectF(0 - penWidth / 2, 0 - penWidth / 2, mWidth + penWidth, mHeight + penWidth);
}

void ResizeableRectItem::hoverMoveEvent( QGraphicsSceneHoverEvent* aEvent )
{ 
    //qDebug("Resize mode = %d", m_ResizeMode);
    if (!(flags() & QGraphicsItem::ItemIsSelectable) || !QGraphicsItem::isSelected())
    {
        SetViewCursor( Qt::OpenHandCursor );
        return;
    }

    const eResize resizeLocation = getResizeLocation( aEvent->pos(), boundingRect() );
    switch ( resizeLocation )
    {
    case eResize_None:
        SetViewCursor( Qt::OpenHandCursor );
        break;

    case eResize_TopLeftCorner:
        SetViewCursor( Qt::SizeFDiagCursor );
        break;

    case eResize_TopRightCorner:
        SetViewCursor( Qt::SizeBDiagCursor );
        break;

    case eResize_BottomLeftCorner:
        SetViewCursor( Qt::SizeBDiagCursor );
        break;

    case eResize_BottomRightCorner:
        SetViewCursor( Qt::SizeFDiagCursor );
        break;

    case eResize_Top:
        SetViewCursor( Qt::SizeVerCursor );
        break;

    case eResize_Left:
        SetViewCursor( Qt::SizeHorCursor );
        break;

    case eResize_Right:
        SetViewCursor( Qt::SizeHorCursor );
        break;

    case eResize_Bottom:
        SetViewCursor( Qt::SizeVerCursor );
        break;
    }
}

void ResizeableRectItem::hoverLeaveEvent( QGraphicsSceneHoverEvent* aEvent )
{
    SetViewCursor( Qt::ArrowCursor );
    QGraphicsItem::hoverLeaveEvent( aEvent );
}

QVariant ResizeableRectItem::itemChange( GraphicsItemChange aChange, const QVariant& aValue )
{
    if ( aChange == ItemPositionHasChanged )
    {
        PosOrRectChanged();
    }
    return QGraphicsItem::itemChange( aChange, aValue );
}

void ResizeableRectItem::Init()
{
    m_ResizeMode = eResize_None;

    setAcceptHoverEvents( true );

    // Change mouse cursor on hover so its easy to see when you can click to move the item
    SetViewCursor( Qt::PointingHandCursor );

    // Allow select and move.
    setFlags( ItemSendsScenePositionChanges | ItemSendsGeometryChanges | ItemIsMovable | ItemIsSelectable );

    UpdateIcon();
}

ResizeableRectItem::eResize ResizeableRectItem::getResizeLocation( QPointF aPos, QRectF aRect )
{
    const auto x = aPos.x();
    const auto y = aPos.y();
    const auto& bRect = aRect;

    const bool xPosNearRectX = IsNear( x, bRect.x() );
    const bool yPosNearRectY = IsNear( y, bRect.y() );
    const bool xPosNearRectW = IsNear( x, bRect.x() + bRect.width() );
    const bool yPosNearRectH = IsNear( y, bRect.y() + bRect.height() );

    // Top right corner
    if ( xPosNearRectW && yPosNearRectY )
    {
        return eResize_TopRightCorner;
    }

    // Bottom left corner
    if ( xPosNearRectX && yPosNearRectH )
    {
        return eResize_BottomLeftCorner;
    }

    // Top left corner
    if ( xPosNearRectX && yPosNearRectY )
    {
        return eResize_TopLeftCorner;
    }

    // Bottom right corner
    if ( xPosNearRectW && yPosNearRectH )
    {
        return eResize_BottomRightCorner;
    }

    // Left edge
    if ( xPosNearRectX && !yPosNearRectY )
    {
        return eResize_Left;
    }

    // Top edge
    if ( !xPosNearRectX && yPosNearRectY )
    {
        return eResize_Top;
    }

    // Right edge
    if ( xPosNearRectW && !yPosNearRectH )
    {
        return eResize_Right;
    }

    // Bottom edge
    if ( !xPosNearRectW && yPosNearRectH )
    {
        return eResize_Bottom;
    }

    return eResize_None;
}

bool ResizeableRectItem::IsNear( qreal xP1, qreal xP2 )
{
    qreal tolerance = 8; // aka epsilon
    if ( mWidth <= kMinRectSize || mHeight <= kMinRectSize )
    {
        tolerance = 1;
    }
    if ( abs( xP1-xP2 ) <= tolerance )
    {
        return true;
    }
    return false;
}

void ResizeableRectItem::onResize( QPointF aPos )
{
    QRectF curRect = CurrentRect();
    const bool isLeft = ( m_ResizeMode == eResize_Left )     || ( m_ResizeMode == eResize_TopLeftCorner )    || ( m_ResizeMode == eResize_BottomLeftCorner );
    const bool isRight = ( m_ResizeMode == eResize_Right )   || ( m_ResizeMode == eResize_TopRightCorner )   || ( m_ResizeMode == eResize_BottomRightCorner );
    const bool isTop = ( m_ResizeMode == eResize_Top )       || ( m_ResizeMode == eResize_TopLeftCorner )    || ( m_ResizeMode == eResize_TopRightCorner );
    const bool isBottom = ( m_ResizeMode == eResize_Bottom ) || ( m_ResizeMode == eResize_BottomLeftCorner ) || ( m_ResizeMode == eResize_BottomRightCorner );

    if ( isRight )
    {
        qreal newWidth = aPos.x() - curRect.x();
        newWidth = mPointSnapper.SnapX(mSnapSettings.MapObjectSnapping().mSnapX, newWidth);
        if ( newWidth < kMinRectSize )
        {
            newWidth = kMinRectSize;
        }
        curRect.setWidth(newWidth);
    }
    else if ( isLeft )
    {
        qreal newx = aPos.x();
        newx = mPointSnapper.SnapX(mSnapSettings.MapObjectSnapping().mSnapX, newx);
        if ( newx > (curRect.x()+curRect.width())-kMinRectSize )
        {
            newx = (curRect.x()+curRect.width())-kMinRectSize;
        }
        curRect.setX(newx);
    }

    if ( isTop )
    {
        qreal newy = aPos.y();
        newy = mPointSnapper.SnapY(mSnapSettings.MapObjectSnapping().mSnapY, newy);
        if ( newy > (curRect.y()+curRect.height()-kMinRectSize))
        {
            newy = curRect.y()+curRect.height()-kMinRectSize;
        }
        curRect.setY(newy);
    }
    else if ( isBottom )
    {
        qreal newHeight = aPos.y() - curRect.y();
        newHeight = mPointSnapper.SnapY(mSnapSettings.MapObjectSnapping().mSnapY, newHeight);
        if ( newHeight < kMinRectSize )
        {
            newHeight = kMinRectSize;
        }
        curRect.setHeight(newHeight);
    }

    SetRect( curRect );
    
    UpdateIcon();
}

void ResizeableRectItem::SetViewCursor(Qt::CursorShape cursor)
{
    mView->setCursor(cursor);
}

QRectF ResizeableRectItem::CurrentRect() const
{
    return QRectF(x(), y(), mWidth, mHeight);
}

void ResizeableRectItem::SetRect(const QRectF& rect)
{
    setX(rect.x());
    setWidth(rect.width());
    setY(rect.y());
    setHeight(rect.height());
    PosOrRectChanged();
    UpdateIcon();
}

void ResizeableRectItem::SyncFromMapObject()
{
    setX(mMapObject->XPos());
    setY(mMapObject->YPos());
    setWidth(mMapObject->Width());
    setHeight(mMapObject->Height());
    UpdateIcon();
}

void ResizeableRectItem::SyncToMapObject()
{
    const auto xpos = static_cast<s32>(pos().x());
    const auto ypos = static_cast<s32>(pos().y());
    mMapObject->SetXPos(xpos);
    mMapObject->SetYPos(ypos);
    mMapObject->mBaseTlv->mBottomRightX = xpos + mWidth;
    mMapObject->mBaseTlv->mBottomRightY = ypos + mHeight;
    UpdateIcon();
}

void ResizeableRectItem::PosOrRectChanged()
{
    SyncToMapObject();

    // Update the property tree view
    mPropSyncer.Sync(this);
}

void ResizeableRectItem::UpdateIcon()
{
    QString images_path = ":/object_images/rsc/object_images/";
    /*
    QString object_name = mMapObject->mObjectStructureType.c_str();
    
    if( object_name == "BirdPortal" )
    {
        if( PropertyByName( "Portal Type", mMapObject->mProperties ))
        {
            if(PropertyByName("Portal Type",mMapObject->mProperties)->mEnumValue == "Abe")
            {
                object_name += "Abe";
            }
            else if(PropertyByName("Portal Type",mMapObject->mProperties)->mEnumValue == "Shrykull")
            {
                object_name += "Shrykull";
            }
        }
    }
    else if( object_name == "Drill" )
    {
        images_path = images_path + object_name + "/";
        object_name += "_";
        
        if( mWidth > 25 )
        {
            object_name += QString::number(std::min( mWidth / 25, 9)) + "_1";
        }
        else
        {
            object_name += "1_" + QString::number(std::min( mHeight / 20, 9));
        }
    }
    else if( object_name == "Edge" || object_name == "Hoist" )
    {
        images_path = images_path + object_name + "/";
        
        if( PropertyByName( "Grab Direction", mMapObject->mProperties ))
        {
            if( PropertyByName( "Grab Direction", mMapObject->mProperties )->mEnumValue == "Facing Right" )
            {
                object_name = "Right";
            }
            else
            {
                object_name = "Left";
            }
        }
    }
    else if( object_name == "MotionDetector" )
    {
        images_path = images_path + object_name + "/";
        object_name = QString::number(std::max(std::min((mWidth / 26), 10), 0));
    }
    else if( object_name == "Mudokon" )
    {
        if( PropertyByName( "Emotion", mMapObject->mProperties ))
        {
            images_path = images_path + object_name + "/";
            object_name = "Mud";
            
            if( PropertyByName( "Emotion", mMapObject->mProperties )->mEnumValue == "Angry" )
            {
                object_name += "Angry";
            }
            else if( PropertyByName( "Emotion", mMapObject->mProperties )->mEnumValue == "Sad" )
            {
                object_name += "Sad";
            }
            else if( PropertyByName( "Emotion", mMapObject->mProperties )->mEnumValue == "Sick" )
            {
                object_name += "Sick";
            }
            else if( PropertyByName( "Emotion", mMapObject->mProperties )->mEnumValue == "Wired" )
            {
                object_name += "Wired";
            }
            else
            {
                object_name += "Normal";
            }
            
            if(PropertyByName("Blind",mMapObject->mProperties)->mEnumValue == "Yes")
            {
                object_name += "B";
            }
        }
    }
    else if( object_name == "UXB" )
    {
        if( PropertyByName( "Start State", mMapObject->mProperties ))
        {
            if(PropertyByName("Start State",mMapObject->mProperties)->mEnumValue == "Off")
            {
                object_name += "disarmed";
            }
        }
    }
    
    if ( !QPixmapCache::find(images_path + object_name + ".png", &m_Pixmap ) )
    {
        m_Pixmap = QPixmap(images_path + object_name + ".png");
        QPixmapCache::insert(images_path + object_name + ".png", m_Pixmap );
    }
    */
}
