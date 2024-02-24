#ifndef CAMERAMANAGER_HPP
#define CAMERAMANAGER_HPP

#include <QDialog>

namespace Ui {
class CameraManager;
}

class QPoint;
class EditorTab;
class CameraGraphicsItem;
struct EditorCamera;

enum TabImageIdx
{
    Main = 0,
    Foreground = 1,
    Background = 2,
    ForegroundWell = 3,
    BackgroundWell = 4,
};

class CameraManager : public QDialog
{
    Q_OBJECT

public:
    CameraManager(QWidget *parent, EditorTab* pParentTab, const QPoint* openedPos);
    ~CameraManager();

    void OnCameraSwapped(EditorCamera* pOld, EditorCamera* pNew);
    void OnCameraIdChanged(EditorCamera* pCam);
    void CreateCamera(bool dropEvent, QPixmap img);

private slots:
    void on_btnSelectImage_clicked();

    void on_btnDeleteImage_clicked();

    void on_btnSetCameraId_clicked();

    void on_btnDeleteCamera_clicked();

    void on_lstCameras_itemSelectionChanged();

private:
    void SetTabImage(int idx, QPixmap img);

    void UpdateTabImages(CameraGraphicsItem* pItem);
    
    int NextFreeCamId();
    bool SaveCameraImage(const QPixmap& camImage, const QString& pathDirectory, const std::string& camName, TabImageIdx imgIdx);

    CameraGraphicsItem* CameraGraphicsItemByPos(const QPoint& pos);
    CameraGraphicsItem* CameraGraphicsItemByModelPtr(const EditorCamera* cam);

    Ui::CameraManager *ui;
    EditorTab* mTab = nullptr;
};

#endif // CAMERAMANAGER_HPP
