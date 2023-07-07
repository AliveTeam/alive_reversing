#pragma once

#include <QGraphicsRectItem>
#include <QPixmap>

struct Camera;

class CameraGraphicsItem final : public QGraphicsRectItem
{
public:
    CameraGraphicsItem(Camera* pCamera, int xpos, int ypos, int width, int height, int transparency);
    void paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget) override;

    const Camera* GetCamera() const
    {
        return mCamera;
    }
    
    Camera* GetCamera()
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

    Camera* mCamera = nullptr;
    struct Images final
    {
        QPixmap mCamera;
    };
    Images mImages;
};
