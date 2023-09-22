#include "CameraGraphicsItem.hpp"
#include <QPen>
#include <QPainter>
#include "Model.hpp"
#include "IGraphicsItem.hpp"

CameraGraphicsItem::CameraGraphicsItem(Camera* pCamera, int xpos, int ypos, int width, int height, int transparency) : QGraphicsRectItem(xpos, ypos, width, height), mCamera(pCamera)
{
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor::fromRgb(120, 120, 120));
    setPen(pen);
    setZValue(1.0);
    LoadImages();
    IGraphicsItem::SetTransparency(this, transparency);
}

void CameraGraphicsItem::paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget)
{
    // Draw the camera image if we have one
    if (!mImages.mCamera.isNull())
    {
        // Account for AOs whacky camera offset, should probably be part of the json schema
        int offX = 0;
        int offY = 0;
        if (rect().width() >= 1024)
        {
            offX = 258;
            offY = 114;
        }

        QRect camImgRect = QRect(rect().x() + offX, rect().y() + offY, 368, 240);

        aPainter->drawPixmap(camImgRect, mImages.mCamera);
    }

    // Draw the rect outline of the camera
    QGraphicsRectItem::paint(aPainter, aOption, aWidget);

    /*
    // Draw the camera name
    if (mCamera && !mCamera->mName.empty())
    {
        aPainter->setBrush(QBrush(QColor::fromRgb(240, 240, 240)));
        aPainter->setPen(Qt::black);
        aPainter->setOpacity(0.8);

        const QRect textRect = QRect(rect().x() + 10, rect().y() + 10, 80, 20);
        aPainter->drawRect(textRect);
        aPainter->drawText(textRect, Qt::AlignCenter, mCamera->mName.c_str());
    }*/
}

void CameraGraphicsItem::LoadImages()
{
    /*/
    if (mCamera)
    {
        if (!mCamera->mCameraImageandLayers.mCameraImage.empty())
        {
            mImages.mCamera.loadFromData(QByteArray::fromBase64(QByteArray(mCamera->mCameraImageandLayers.mCameraImage.c_str(), static_cast<int>(mCamera->mCameraImageandLayers.mCameraImage.length()))));
        }
    }*/

}