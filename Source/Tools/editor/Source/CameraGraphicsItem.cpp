#include "CameraGraphicsItem.hpp"
#include <QPen>
#include <QPainter>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include "Model.hpp"
#include "IGraphicsItem.hpp"

CameraGraphicsItem::CameraGraphicsItem(EditorCamera* pCamera, int xpos, int ypos, int width, int height, int transparency)
    : QGraphicsRectItem(xpos, ypos, width, height), mCamera(pCamera)
{
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor::fromRgb(120, 120, 120));
    setPen(pen);
    setZValue(1.0);
    IGraphicsItem::SetTransparency(this, transparency);
}

void CameraGraphicsItem::paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget)
{
    // Draw the camera image if we have one
    if (!mCamera->mCameraImageandLayers.mCameraImage.isNull())
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

        aPainter->drawPixmap(camImgRect, mCamera->mCameraImageandLayers.mCameraImage);
    }

    // Draw the rect outline of the camera
    QGraphicsRectItem::paint(aPainter, aOption, aWidget);

    // Draw the camera name
    if (mCamera && !mCamera->mName.empty())
    {
        aPainter->setBrush(QBrush(QColor::fromRgb(240, 240, 240)));
        aPainter->setPen(Qt::black);
        aPainter->setOpacity(0.8);

        const QRect textRect = QRect(rect().x() + 10, rect().y() + 10, 80, 20);
        aPainter->drawRect(textRect);
        aPainter->drawText(textRect, Qt::AlignCenter, mCamera->mName.c_str());
    }
}

static QByteArray ReadAll(QString fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::OpenModeFlag::ReadOnly))
    {
        // TODO handle error
        return {};
    }
    return f.readAll();
}

void CameraGraphicsItem::SetImage(QPixmap image)
{
     mCamera->mCameraImageandLayers.mCameraImage = image;
}

QPixmap CameraGraphicsItem::GetImage()
{
    return mCamera->mCameraImageandLayers.mCameraImage;
}

void CameraGraphicsItem::Load(QString basePath)
{
    if (mCamera->mName.empty())
    {
        return;
    }

    QString baseCameraName = basePath + "/" + mCamera->mName.c_str(); // e.g barracks/paths/1/P01C04
    qDebug() << baseCameraName;

    mCamera->mCameraImageandLayers.mCameraImage.loadFromData(ReadAll(baseCameraName + ".png"));

    QString fileName = baseCameraName + ".json";
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        // TODO: should prob be a fatal error or added to a list of "problems" to be shown
        // when the path is loaded
        return;
    }

    QTextStream in(&f);
    const std::string jsonStr = in.readAll().toStdString();
    if (!jsonStr.empty())
    {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        // TODO
        //newRes.mFg1ResBlockCount = j["fg1_block_count"];

        for (auto& fg1File : j["layers"])
        {
            std::string s = fg1File;
            if (s.find("fg_well") != std::string::npos)
            {
                mCamera->mCameraImageandLayers.mForegroundWellLayer.loadFromData(ReadAll(baseCameraName + "fg_well.png"));
            }
            else if (s.find("bg_well") != std::string::npos)
            {
                mCamera->mCameraImageandLayers.mBackgroundWellLayer.loadFromData(ReadAll(baseCameraName + "bg_well.png"));
            }
            else if (s.find("fg") != std::string::npos)
            {
                mCamera->mCameraImageandLayers.mForegroundLayer.loadFromData(ReadAll(baseCameraName + "fg.png"));
            }
            else if (s.find("bg") != std::string::npos)
            {
                mCamera->mCameraImageandLayers.mBackgroundLayer.loadFromData(ReadAll(baseCameraName + "bg.png"));
            }
        }
    }
}
