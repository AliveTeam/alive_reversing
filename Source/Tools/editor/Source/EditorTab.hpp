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
#include "SnapSettings.hpp"
#include <QGraphicsView>

namespace Ui
{
    class EditorTab;
}

class EditorGraphicsScene;
class ResizeableArrowItem;
class ResizeableRectItem;
class CameraGraphicsItem;
class MapObjectBase;
class CollisionObject;
class CameraManager;
class ClipBoard;
class SnapSettings;
struct EditorCamera;
class Model;

class EditorTab final : public QMainWindow, public IPointSnapper
{
    Q_OBJECT
public:
    EditorTab(QTabWidget* aParent, std::unique_ptr<Model> model, QString jsonFileName, bool isTempFile, QStatusBar* pStatusBar, SnapSettings& snapSettings);
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

    ResizeableRectItem* MakeResizeableRectItem(MapObjectBase* pMapObject);
    ResizeableArrowItem* MakeResizeableArrowItem(CollisionObject* pCollisionObject);
    CameraGraphicsItem* MakeCameraGraphicsItem(EditorCamera* pCamera, int x, int y, int w, int h);

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

    QString GetPathDirectory() const
    {
        return mPathDirectory;
    }

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
    std::unique_ptr<Model> mModel;
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
    QString mPathDirectory;
};

class EditorGraphicsView final : public QGraphicsView
{
    Q_OBJECT

public:
    EditorGraphicsView(EditorTab* editorTab);

    void mousePressEvent(QMouseEvent* pEvent) override;
    void mouseReleaseEvent(QMouseEvent* pEvent) override;
    void wheelEvent(QWheelEvent* pEvent) override;

    // TODO: implement proper ScrollHandDrag mode.
    // you should be able to move around by pressing and holding the middle mouse button.
    void keyPressEvent(QKeyEvent* pEvent) override;

    void keyReleaseEvent(QKeyEvent* pEvent) override;
    void focusOutEvent(QFocusEvent* pEvent) override;
    void contextMenuEvent(QContextMenuEvent* pEvent) override;
    void dragEnterEvent(QDragEnterEvent* pEvent) override;
    void dragMoveEvent(QDragMoveEvent* pEvent) override;
    void dropEvent(QDropEvent* pEvent) override;

private:
    EditorTab* mEditorTab = nullptr;
};
