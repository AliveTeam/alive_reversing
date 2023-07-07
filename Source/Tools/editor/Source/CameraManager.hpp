#ifndef CAMERAMANAGER_HPP
#define CAMERAMANAGER_HPP

#include <QDialog>
#include <QPoint>

namespace Ui {
class CameraManager;
}

class EditorTab;
class CameraGraphicsItem;
struct Camera;

class CameraManager : public QDialog
{
    Q_OBJECT

public:
    CameraManager(QWidget *parent, EditorTab* pParentTab, const QPoint* openedPos);
    ~CameraManager();

    void OnCameraSwapped(Camera* pOld, Camera* pNew);
    void OnCameraIdChanged(Camera* pCam);
    void CreateCamera(bool dropEvent, QPixmap img);

private slots:
    void on_btnExportImage_clicked();

    void on_btnSelectImage_clicked();

    void on_btnDeleteImage_clicked();

    void on_btnSetCameraId_clicked();

    void on_btnDeleteCamera_clicked();

    void on_lstCameras_itemSelectionChanged();

private:
    void SetTabImage(int idx, QPixmap img);

    void UpdateTabImages(CameraGraphicsItem* pItem);
    
    int NextFreeCamId();

    CameraGraphicsItem* CameraGraphicsItemByPos(const QPoint& pos);
    CameraGraphicsItem* CameraGraphicsItemByModelPtr(const Camera* cam);

    Ui::CameraManager *ui;
    EditorTab* mTab = nullptr;
};

#endif // CAMERAMANAGER_HPP
