#pragma once

#include <QGraphicsRectItem>
#include <QPixmap>
#include "Model.hpp"

class CameraGraphicsItem final : public QGraphicsRectItem
{
public:
    CameraGraphicsItem(Model::Camera* pCamera, int xpos, int ypos, int width, int height, int transparency);
    void paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget) override;

    const Model::Camera* GetCamera() const
    {
        return mCamera;
    }
    
    Model::Camera* GetCamera()
    {
        return mCamera;
    }

    void SetImage(QPixmap image)
    {
        mImages.mCamera = image;
    }

    QPixmap GetImage()
    {
        return mImages.mCamera;
    }
private:
    void LoadImages();

    Model::Camera* mCamera = nullptr;
    struct Images final
    {
        QPixmap mCamera;
    };
    Images mImages;
};
