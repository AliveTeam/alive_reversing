#pragma once

#include <QMainWindow>
#include <QDockWidget>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QUndoStack>
#include <QPainter>
#include <QTreeWidget>
#include <QApplication>
#include <memory>
#include "Model.hpp"
#include "SnapSettings.hpp"

namespace Ui
{
    class EditorTab;
}

class EditorGraphicsScene;
class ResizeableArrowItem;
class ResizeableRectItem;
class CameraGraphicsItem;
struct MapObject;
class CollisionObject;
class CameraManager;
class ClipBoard;
class SnapSettings;

class EditorTab final : public QMainWindow, public IPointSnapper
{
    Q_OBJECT
public:
    EditorTab(QTabWidget* aParent, UP_Model model, QString jsonFileName, bool isTempFile, QStatusBar* pStatusBar, SnapSettings& snapSettings);
    ~EditorTab();
    void ZoomIn();
    void ZoomOut();
    void ResetZoom();
    bool Save();
    bool SaveAs();
    void Export(bool exportAndPlay);
    QString GetJsonFileName() const { return mJsonFileName; }
    Model& GetModel() const { return *mModel; }
    void ClearPropertyEditor();
    void PopulatePropertyEditor(QGraphicsItem* pItem);
    void Undo();
    void Redo();
    void wheelEvent(QWheelEvent* pEvent) override;
    EditorGraphicsScene& GetScene() 
    {
        return *mScene;
    }

    void EditHintFlyMessages();
    void EditLCDScreenMessages();
    void EditPathData();
    void EditMapSize();

    void AddCommand(QUndoCommand* pCmd)
    {
        mUndoStack.push(pCmd);
    }

    bool IsClean() const
    {
        return !mIsTempFile && mUndoStack.isClean();
    }

    void UpdateCleanState();
    void UpdateTabTitle(bool clean);

    void AddObject();
    void AddCollision();

    void ConnectCollisions();

    ResizeableRectItem* MakeResizeableRectItem(MapObject* pMapObject);
    ResizeableArrowItem* MakeResizeableArrowItem(CollisionObject* pCollisionObject);
    CameraGraphicsItem* MakeCameraGraphicsItem(Camera* pCamera, int x, int y, int w, int h);

    CameraManager* GetCameraManagerDialog()
    {
        return mCameraManager;
    }

    void SetCameraManagerDialog(CameraManager* pDlg)
    {
        mCameraManager = pDlg;
    }

    void SyncPropertyEditor();

    void EditTransparency();

    void Cut(ClipBoard& clipBoard);
    void Copy(ClipBoard& clipBoard);
    void Paste(ClipBoard& clipBoard);

signals:
    void CleanChanged();

private slots:

    void cleanChanged(bool clean);

private:
    bool DoSave(QString fileName);

    int SnapX(bool enabled, int x) override;
    int SnapY(bool enabled, int y) override;

    Ui::EditorTab* ui = nullptr;
    float iZoomLevel = 1.0f;
    UP_Model mModel;
    QUndoStack mUndoStack;
    std::unique_ptr<EditorGraphicsScene> mScene;
    QString mJsonFileName;

    QString mExportedPathLvlName;
    QString mReliveExePath;
    QString mExtraLvlsPath;
    QTabWidget* mParent = nullptr;
    bool mIsTempFile = false;

    CameraManager* mCameraManager = nullptr;

    QStatusBar* mStatusBar = nullptr;

    SnapSettings& mSnapSettings;
};
