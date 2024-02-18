#pragma once

#include <QGraphicsRectItem>
#include <QPixmap>

struct EditorCamera;

class CameraGraphicsItem final : public QGraphicsRectItem
{
public:
    CameraGraphicsItem(EditorCamera* pCamera, int xpos, int ypos, int width, int height, int transparency);
    void paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget) override;

    const EditorCamera* GetCamera() const
    {
        return mCamera;
    }
    
    EditorCamera* GetCamera()
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

    EditorCamera* mCamera = nullptr;
    struct Images final
    {
        QPixmap mCamera;
    };
    Images mImages;
};
