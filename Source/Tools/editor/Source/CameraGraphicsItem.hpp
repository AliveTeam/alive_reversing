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

    void SetImage(QPixmap image);
    QPixmap GetImage();

    void Load(QString basePath);

private:
    EditorCamera* mCamera = nullptr;
};
