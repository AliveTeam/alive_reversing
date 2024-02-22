#include "EditorTab.hpp"
#include "ui_EditorTab.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QOpenGLWidget>
#include <QUndoCommand>
#include <QSpinBox>
#include <QMenu>
#include <QStatusBar>
#include <QFileDialog>
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"
#include "CameraGraphicsItem.hpp"
#include "EditorGraphicsScene.hpp"
#include <QLineEdit>
#include "StringProperty.hpp"
#include "PropertyTreeWidget.hpp"
#include <QFileInfo>
#include "BasicTypeProperty.hpp"
#include "EnumProperty.hpp"
#include "CameraManager.hpp"
#include "ExportPathDialog.hpp"
#include "ChangeMapSizeDialog.hpp"
#include "MessageEditorDialog.hpp"
#include "PathDataEditorDialog.hpp"
#include "AddObjectDialog.hpp"
#include "SelectionSaver.hpp"
#include "TransparencyDialog.hpp"
#include "ProgressDialog.hpp"
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "DeleteItemsCommand.hpp"
#include "ClipBoard.hpp"
#include "../../../relive_lib/Grid.hpp"
#include "CollisionConnect.hpp"
#include "Model.hpp"

// Zoom by 10% each time.
const float KZoomFactor = 0.10f;
const float KMaxZoomOutLevels = 6.0f;
const float KMaxZoomInLevels = 14.0f;

//INITIALIZE_EASYLOGGINGPP

class SetSelectionCommand final : public QUndoCommand
{
    Q_DECLARE_TR_FUNCTIONS(SetSelectionCommand)

public:
    SetSelectionCommand(EditorTab* pTab, QGraphicsScene* pScene, QList<QGraphicsItem*>& oldSelection, QList<QGraphicsItem*>& newSelection) 
      : mTab(pTab),
        mScene(pScene),
        mOldSelection(oldSelection),
        mNewSelection(newSelection)
    {
        mFirst = true;
        if (mNewSelection.count() > 0)
        {
            setText(tr("Select %1 item(s)").arg(mNewSelection.count()));
        }
        else
        {
            setText(tr("Clear selection"));
        }
    }

    void redo() override
    {
        if (!mFirst)
        {
            mScene->clearSelection();
            for (auto& item : mNewSelection)
            {
                item->setSelected(true);
            }
            mScene->update();
        }
        mFirst = false;
        mTab->SyncPropertyEditor();
    }

    void undo() override
    {
        mScene->clearSelection();
        for (auto& item : mOldSelection)
        {
            item->setSelected(true);
        }
        mScene->update();
        mTab->SyncPropertyEditor();
    }

private:
    EditorTab* mTab = nullptr;
    QGraphicsScene* mScene = nullptr;
    QList<QGraphicsItem*> mOldSelection;
    QList<QGraphicsItem*> mNewSelection;
    bool mFirst = false;
};


class MoveItemsCommand final : public QUndoCommand
{
    Q_DECLARE_TR_FUNCTIONS(MoveItemsCommand)

public:
    MoveItemsCommand(QGraphicsScene* pScene, ItemPositionData oldPositions, ItemPositionData newPositions, Model& model)
        : mScene(pScene),
        mOldPositions(oldPositions),
        mNewPositions(newPositions),
        mModel(model)
    {
        mFirst = true;

        if (mNewPositions.Count() == 1)
        {
            auto pNewLine = mNewPositions.FirstLinePos();
            auto pOldRect = mOldPositions.FirstRectPos();
            if (pNewLine)
            {
                auto pOldLine = mOldPositions.FirstLinePos();
                const bool posChange = pOldLine->x != pNewLine->x || pOldLine->y != pNewLine->y;
                const bool lineChanged = pOldLine->line != pNewLine->line;
                if (posChange && lineChanged)
                {
                    setText(tr("Move and resize collision"));
                }
                else if (posChange && !lineChanged)
                {
                    setText(tr("Move collision"));
                }
                else
                {
                    setText(tr("Move collision point"));
                }
            }
            else
            {
                auto pNewRect = mNewPositions.FirstRectPos();
                const bool xOryChanged = pOldRect->rect.x() != pNewRect->rect.x() || pOldRect->rect.y() != pNewRect->rect.y();
                const bool wOrhChanged = pOldRect->rect.width() != pNewRect->rect.width() || pOldRect->rect.height() != pNewRect->rect.height();
                if (xOryChanged && wOrhChanged)
                {
                    setText(tr("Move and resize map object"));
                }
                else if (xOryChanged && !wOrhChanged)
                {
                    setText(tr("Move map object"));
                }
                else // only wOrhChanged
                {
                    setText(tr("Resize map object"));
                }
            }
        }
        else
        {
            setText(tr("Move %1 item(s)").arg(mNewPositions.Count()));
        }
    }

    void redo() override
    {
        if (!mFirst)
        {
            mNewPositions.Restore(mModel);
            mScene->update();
        }
        mFirst = false;
    }

    void undo() override
    {
        mOldPositions.Restore(mModel);
        mScene->update();
    }

private:
    QGraphicsScene* mScene = nullptr;
    ItemPositionData mOldPositions;
    ItemPositionData mNewPositions;
    Model& mModel;
    bool mFirst = false;
};


EditorGraphicsView::EditorGraphicsView(EditorTab* editorTab)
    : mEditorTab(editorTab)
{
    setAcceptDrops(true);
}

void EditorGraphicsView::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
    {
        // prevent band dragging on other buttons
        qDebug() << "Ignore non left press";
        pEvent->ignore();
        return;
    }

    qDebug() << "view mouse press (left)";
    QGraphicsView::mousePressEvent(pEvent);
}

void EditorGraphicsView::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
    {
        qDebug() << "Ignore non left release";
        pEvent->ignore();
        return;
    }

    qDebug() << "view mouse release (left)";
    QGraphicsView::mouseReleaseEvent(pEvent);
}

void EditorGraphicsView::wheelEvent(QWheelEvent* pEvent)
{
    if (pEvent->modifiers() == Qt::Modifier::CTRL)
    {
        pEvent->ignore();
        return;
    }
    QGraphicsView::wheelEvent(pEvent);
}


// TODO: implement proper ScrollHandDrag mode.
// you should be able to move around by pressing and holding the middle mouse button.
void EditorGraphicsView::keyPressEvent(QKeyEvent* pEvent)
{
    if (pEvent->key() == Qt::Key::Key_Shift)
    {
        setDragMode(DragMode::ScrollHandDrag);
        setInteractive(false);
        pEvent->ignore();
        return;
    }
    QGraphicsView::keyPressEvent(pEvent);
}

void EditorGraphicsView::keyReleaseEvent(QKeyEvent* pEvent)
{
    if (pEvent->key() == Qt::Key::Key_Shift)
    {
        setDragMode(DragMode::RubberBandDrag);
        setInteractive(true);
        pEvent->ignore();
        return;
    }
    QGraphicsView::keyPressEvent(pEvent);
}

void EditorGraphicsView::focusOutEvent(QFocusEvent* pEvent)
{
    if (pEvent->lostFocus())
    {
        // prevents ScrollHandDrag getting "stuck" when losing focus while holding shift
        setDragMode(DragMode::RubberBandDrag);
        setInteractive(true);
    }
    QGraphicsView::focusOutEvent(pEvent);
}

void EditorGraphicsView::contextMenuEvent(QContextMenuEvent* pEvent)
{
    QMenu menu(this);

    const QPoint scenePos = mapToScene(pEvent->pos()).toPoint();
    QList<QGraphicsItem*> itemsAtMousePos = items(scenePos);
    qDebug() << "There are " << itemsAtMousePos.count() << " at the context menu";

    bool cameraAtMenu = false;
    for (s32 i = 0; i < itemsAtMousePos.count(); i++)
    {
        auto pCameraItem = qgraphicsitem_cast<CameraGraphicsItem*>(itemsAtMousePos[i]);
        if (pCameraItem)
        {
            cameraAtMenu = true;
            break;
        }
    }

    if (cameraAtMenu)
    {
        auto pEditCameraAction = new QAction(tr("Edit camera"), &menu);
        connect(pEditCameraAction, &QAction::triggered, this, [&]()
                {
                    CameraManager cameraManager(this, mEditorTab, &scenePos);
                    mEditorTab->SetCameraManagerDialog(&cameraManager);
                    cameraManager.exec();
                    mEditorTab->SetCameraManagerDialog(nullptr); });
        menu.addAction(pEditCameraAction);
    }

    if (scene()->selectedItems().count() > 0)
    {
        // TODO: Copy

        // TODO: Cut

        // TODO: Check if the selection has collision items
        // If nothing is selected its impossible to connect collision items
        auto pConnectCollisionsAction = new QAction(tr("Connect collisions"), &menu);
        connect(pConnectCollisionsAction, &QAction::triggered, this, [&]()
                { mEditorTab->ConnectCollisions(); });
        menu.addAction(pConnectCollisionsAction);
    }

    // TODO: Show paste if the global clipboard object owned by MainWindow isn't empty

    if (menu.actions().count() > 0)
    {
        menu.exec(pEvent->globalPos());
    }
}

void EditorGraphicsView::dragEnterEvent(QDragEnterEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void EditorGraphicsView::dragMoveEvent(QDragMoveEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void EditorGraphicsView::dropEvent(QDropEvent* pEvent)
{
    // Attempt to load the dropped image
    QUrl imgUrl = pEvent->mimeData()->urls().first();
    QPixmap img;

    if (!imgUrl.isLocalFile())
    {
        QMessageBox::critical(this, "Error", "Reading from remote file systems is unsupported.");
        return;
    }

    if (!img.load(imgUrl.toLocalFile()))
    {
        QMessageBox::critical(this, "Error", "The file dropped could not be understood as an image.");
        return;
    }

    // Image is valid, continue
    const QPoint scenePos = mapToScene(pEvent->pos()).toPoint();
    CameraManager cameraManager(this, mEditorTab, &scenePos);
    cameraManager.CreateCamera(true, img);
    pEvent->acceptProposedAction();
}

EditorTab::EditorTab(QTabWidget* aParent, std::unique_ptr<Model> model, QString jsonFileName, bool isTempFile, QStatusBar* pStatusBar, SnapSettings& snapSettings)
    : QMainWindow(aParent),
    ui(new Ui::EditorTab),
    mModel(std::move(model)),
    mJsonFileName(jsonFileName),
    mParent(aParent),
    mIsTempFile(isTempFile),
    mStatusBar(pStatusBar),
    mSnapSettings(snapSettings)
{
    ui->setupUi(this);

    // TODO: Set as a promoted type
    delete ui->graphicsView;
    ui->graphicsView = new EditorGraphicsView(this);
    QGraphicsView* pView = ui->graphicsView;
    pView->setDragMode(QGraphicsView::RubberBandDrag);


    pView->setRenderHint(QPainter::SmoothPixmapTransform);
    pView->setRenderHint(QPainter::Antialiasing);
    pView->setRenderHint(QPainter::TextAntialiasing);

    // turn off for now because of performance issues when a lot of
    // objects are onscreen
    //pView->setViewport(new QOpenGLWidget()); // Becomes owned by the view

    mScene = std::make_unique<EditorGraphicsScene>(this);

    connect(mScene.get(), &EditorGraphicsScene::SelectionChanged, this, [&](QList<QGraphicsItem*> oldSelection, QList<QGraphicsItem*> newSelection)
        {
            mUndoStack.push(new SetSelectionCommand(this, mScene.get(), oldSelection, newSelection));
        });

    connect(mScene.get(), &EditorGraphicsScene::ItemsMoved, this, [&](ItemPositionData oldPositions, ItemPositionData newPositions)
        {
            mUndoStack.push(new MoveItemsCommand(mScene.get(), oldPositions, newPositions, *mModel));
        });

    connect(&mUndoStack, &QUndoStack::cleanChanged, this, &EditorTab::cleanChanged);

    iZoomLevel = 1.0f;
    for (int i = 0; i < 2; ++i)
    {
        //  ZoomIn();
    }

    setCentralWidget(ui->graphicsView);

    // TODO: Temp hack
    delete ui->treeWidget;
    ui->treeWidget = new PropertyTreeWidget(ui->dockWidgetContents_2);
    ui->verticalLayout_5->addWidget(ui->treeWidget);

    // Disable "already disabled" context menus on the QDockWidgets
    ui->propertyDockWidget->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->undoHistoryDockWidget->setContextMenuPolicy(Qt::PreventContextMenu);

    QFileInfo fi(jsonFileName);

    for (u32 x = 0; x < mModel->XSize(); x++)
    {
        for (u32 y = 0; y < mModel->YSize(); y++)
        {
            EditorCamera* pCam = mModel->CameraAt(x, y);
            auto pCameraGraphicsItem = MakeCameraGraphicsItem(pCam, mModel->CameraGridWidth() * x, y * mModel->CameraGridHeight(), mModel->CameraGridWidth(), mModel->CameraGridHeight());
            pCameraGraphicsItem->Load(fi.dir().absolutePath());
            mScene->addItem(pCameraGraphicsItem);

            if (pCam)
            {
                for (auto& mapObj : pCam->mMapObjects)
                {
                    auto pMapObject = MakeResizeableRectItem(mapObj.get());
                    mScene->addItem(pMapObject);
                }
            }
        }
    }


    for (auto& collision : mModel->CollisionItems())
    {
        auto pLine = MakeResizeableArrowItem(collision.get());
        mScene->addItem(pLine);
    }

    mScene->UpdateSceneRect();

    ui->graphicsView->setScene(mScene.get());

    mUndoStack.setUndoLimit(100);
    ui->undoView->setStack(&mUndoStack);

    static_cast<PropertyTreeWidget*>(ui->treeWidget)->Init();

    addDockWidget(Qt::RightDockWidgetArea, ui->propertyDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, ui->undoHistoryDockWidget);

    ui->propertyDockWidget->setMinimumWidth(310);

    setContextMenuPolicy(Qt::PreventContextMenu);

    connect(&mUndoStack , &QUndoStack::cleanChanged, this, &EditorTab::UpdateTabTitle);
}

ResizeableRectItem* EditorTab::MakeResizeableRectItem(MapObjectBase* pMapObject)
{
    return new ResizeableRectItem(ui->graphicsView, pMapObject, *static_cast<PropertyTreeWidget*>(ui->treeWidget), mScene->GetTransparencySettings().MapObjectTransparency(), mSnapSettings, *this);
}

ResizeableArrowItem* EditorTab::MakeResizeableArrowItem(CollisionObject* pCollisionObject)
{
    return new ResizeableArrowItem(ui->graphicsView, pCollisionObject, *static_cast<PropertyTreeWidget*>(ui->treeWidget), mScene->GetTransparencySettings().CollisionTransparency(), mSnapSettings, *this);
}

CameraGraphicsItem* EditorTab::MakeCameraGraphicsItem(EditorCamera* pCamera, int x, int y, int w, int h)
{
    return new CameraGraphicsItem(pCamera, x, y, w, h, mScene->GetTransparencySettings().CameraTransparency());
}

void EditorTab::SyncPropertyEditor()
{
    auto selected = mScene->selectedItems();
    if (selected.count() == 1)
    {
        PopulatePropertyEditor(selected[0]);
    }
    else
    {
        ClearPropertyEditor();
    }
}

void EditorTab::EditTransparency()
{
    auto transparencyDialog = new TransparencyDialog(this, this);
    transparencyDialog->exec();
    delete transparencyDialog;
}

void EditorTab::Cut(ClipBoard& clipBoard)
{
    if (!mScene->selectedItems().isEmpty())
    {
        clipBoard.Set(mScene->selectedItems(), *mModel);
        mUndoStack.push(new DeleteItemsCommand(this, true, mScene->selectedItems()));
        mStatusBar->showMessage(tr("Selection cut"), 2000);
    }
}

void EditorTab::Copy(ClipBoard& clipBoard)
{
    if (!mScene->selectedItems().isEmpty())
    {
        clipBoard.Set(mScene->selectedItems(), *mModel);
        mStatusBar->showMessage(tr("Selection copied"), 2000);
    }
}

void EditorTab::Paste(ClipBoard& clipBoard)
{
    if (!clipBoard.IsEmpty())
    {
        mUndoStack.push(new PasteItemsCommand(this, clipBoard));
        mStatusBar->showMessage(tr("Items pasted"), 2000);
    }
}

void EditorTab::cleanChanged(bool /*clean*/)
{
    UpdateCleanState();
}

void EditorTab::UpdateTabTitle(bool clean)
{
    QFileInfo fileInfo(mJsonFileName);
    QString title = fileInfo.fileName();
    if (!clean || mIsTempFile)
    {
        title += "*";
    }

    for (int i = 0; i < mParent->count(); i++)
    {
        if (mParent->widget(i) == this)
        {
            mParent->setTabText(i, title);
            break;
        }
    }
}

void EditorTab::wheelEvent(QWheelEvent* pEvent)
{
    if (pEvent->modifiers() == Qt::Modifier::CTRL)
    {
        if (pEvent->angleDelta().y() > 0)
        {
            ZoomIn();
        }
        else
        {
            ZoomOut();
        }
    }
    QWidget::wheelEvent(pEvent);
}

// TODO: zoom doesn't work properly without ui->graphicsView->resetTransform().
// figure out why and if there's a better way.
void EditorTab::ZoomIn()
{
    if (iZoomLevel < 1.0f + (KZoomFactor*KMaxZoomInLevels))
    {
        iZoomLevel += KZoomFactor;
        ui->graphicsView->resetTransform();
        ui->graphicsView->setTransform(QTransform::fromScale(iZoomLevel, iZoomLevel), true);
    }
}

void EditorTab::ZoomOut()
{
    if (iZoomLevel > 1.0f - (KZoomFactor*KMaxZoomOutLevels))
    {
        iZoomLevel -= KZoomFactor;
        ui->graphicsView->resetTransform();
        ui->graphicsView->setTransform(QTransform::fromScale(iZoomLevel, iZoomLevel), true);
    }
}

void EditorTab::ResetZoom()
{
    iZoomLevel = 1.0f;
    ui->graphicsView->resetTransform();
    ui->graphicsView->setTransform(QTransform::fromScale(iZoomLevel, iZoomLevel), true);
}

EditorTab::~EditorTab()
{
    disconnect(&mUndoStack, &QUndoStack::cleanChanged, this, &EditorTab::UpdateTabTitle);
    delete ui;
}

void EditorTab::ClearPropertyEditor()
{
    auto pTree = static_cast<PropertyTreeWidget*>(ui->treeWidget);
    pTree->DePopulate();
}

void EditorTab::PopulatePropertyEditor(QGraphicsItem* pItem)
{
    ClearPropertyEditor();

    auto pTree = static_cast<PropertyTreeWidget*>(ui->treeWidget);
    pTree->Populate(mUndoStack, pItem);
}

void EditorTab::Undo()
{
    mUndoStack.undo();
}

void EditorTab::Redo()
{
    mUndoStack.redo();
}

bool EditorTab::Save()
{
    if (mIsTempFile)
    {
        return SaveAs();
    }
    else
    {
        return DoSave(mJsonFileName);
    }
}

bool EditorTab::SaveAs()
{
    QString jsonSaveFileName = QFileDialog::getSaveFileName(this, tr("Save " + mJsonFileName.toLocal8Bit() + " as json"), "", tr("Json Files (*.json);;All Files (*)"));
    if (jsonSaveFileName.isEmpty())
    {
        // They didn't want to save it
        return false;
    }

    // Append .json file ext if not specified
    if (!jsonSaveFileName.endsWith(".json", Qt::CaseInsensitive))
    {
        jsonSaveFileName += ".json";
    }

    if (!DoSave(jsonSaveFileName))
    {
        return false;
    }

    // Saved OK, update the file name and tab title
    mJsonFileName = jsonSaveFileName;

    // No longer a temp file so don't force SaveAs next time
    if (mIsTempFile)
    {
        mIsTempFile = false;
        UpdateCleanState();
        UpdateTabTitle(true);
    }

    return true;
}

template<class ResultType>
static ResultType ExecASync(QString dialogTitle, std::function<ResultType()> fnDoWork)
{
    auto dlg = new ProgressDialog();
    dlg->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    dlg->setWindowTitle(dialogTitle);

    QFutureWatcher<ResultType>* watcher = new QFutureWatcher<ResultType>();
    QObject::connect(watcher, &QFutureWatcher<ResultType>::finished, [&]() { dlg->close(); });
    QFuture<ResultType> f = QtConcurrent::run(fnDoWork);

    watcher->setFuture(f);

    dlg->exec();

    return f.result();
}

bool EditorTab::DoSave(QString fileName)
{
    if (ExecASync<bool>("Saving... " + fileName, [&]()
        {
            std::string json = mModel->ToJson();
            QFile f(fileName);
            if (f.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&f);
                stream << json.c_str();
                return true;
            }
            return false;
        }))
    {
        mUndoStack.setClean();
        mStatusBar->showMessage(tr("Saved"), 2000);
        return true;
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to save " + fileName);
        mStatusBar->showMessage(tr("Save failed"));
        return false;
    }
}

void EditorTab::Export(bool /*exportAndPlay*/)
{
    /*
    if (!IsClean())
    {
        Save();
    }

    auto exportDialog = new ExportPathDialog(this, exportAndPlay);
    exportDialog->setJsonPath(mJsonFileName);
    exportDialog->setRelivePath(mReliveExePath);
    exportDialog->setExtraLvlsPath(mExtraLvlsPath);

    QFileInfo json(mJsonFileName);
    if (mExportedPathLvlName.isEmpty())
    {
        // will be XXPATH.BND, extract XX
        QString lvlName = QString::fromStdString(mModel->GetMapInfo().mPathBnd).toLower();
        if (lvlName.length() > 2)
        {
            lvlName = lvlName.left(2) + ".lvl";
        }

        if (json.exists())
        {
            lvlName.insert(0, json.path() + "/");
        }
        exportDialog->setLvlName(lvlName);
    }
    else
    {
        exportDialog->setLvlName(mExportedPathLvlName);
    }

    if (mReliveExePath.isEmpty() && json.exists())
    {
        #ifdef Q_OS_WINDOWS
            exportDialog->setRelivePath(json.path() + "/relive.exe");
        #else
            exportDialog->setRelivePath(json.path());
        #endif
    }
    else
    {
        exportDialog->setRelivePath(mReliveExePath);
    }

    exportDialog->exec();

    if (!exportDialog->getLvlName().isEmpty())
    {
        mExportedPathLvlName = exportDialog->getLvlName();
    }

    if (!exportDialog->getRelivePath().isEmpty())
    {
        mReliveExePath = exportDialog->getRelivePath();
    }

    if (!exportDialog->getExtraLvlsPath().isEmpty())
    {
        mExtraLvlsPath = exportDialog->getExtraLvlsPath();
    }

    delete exportDialog;
    */
}

void EditorTab::EditHintFlyMessages()
{
    auto pDlg = new MessageEditorDialog(this, this, *mModel, false);
    pDlg->exec();
    delete pDlg;
}

void EditorTab::EditLCDScreenMessages()
{
    auto pDlg = new MessageEditorDialog(this, this, *mModel, true);
    pDlg->exec();
    delete pDlg;
}

void EditorTab::EditPathData()
{
    auto pDlg = new PathDataEditorDialog(this, this);
    pDlg->exec();
    delete pDlg;
}

void EditorTab::EditMapSize()
{
    auto pDlg = new ChangeMapSizeDialog(this, this);
    pDlg->exec();
    delete pDlg;
}

void EditorTab::UpdateCleanState()
{
    emit CleanChanged();
}

void EditorTab::AddObject()
{
    auto pDlg = new AddObjectDialog(this, this);
    pDlg->exec();
    delete pDlg;
}

class AddCollisionCommand final : public QUndoCommand
{
public:
    explicit AddCollisionCommand(EditorTab* pTab)
     : mSelectionSaver(pTab), mTab(pTab)
    {
        MakeNewCollision();

        setText("Add collision line");
    }

    ~AddCollisionCommand()
    {
        if (!mAdded)
        {
            delete mArrowItem;
        }
    }

    void undo() override
    {
        mTab->GetScene().removeItem(mArrowItem);

        mNewObject = mTab->GetModel().RemoveCollisionItem(mArrowItem->GetCollisionItem());

        mAdded = false;

        mSelectionSaver.undo();
    }

    void redo() override
    {
        mTab->GetScene().addItem(mArrowItem);
        mTab->GetModel().CollisionItems().push_back(std::move(mNewObject));

        // Set the new item as the only thing selected
        mTab->GetScene().clearSelection();
        mArrowItem->setSelected(true);

        mAdded = true;

        mSelectionSaver.redo();
    }

private:
    void MakeNewCollision()
    {
        mNewObject = std::make_unique<CollisionObject>(mTab->GetModel().NextCollisionId());

        QGraphicsView* pView = mTab->GetScene().views().at(0);
        QPoint scenePos = pView->mapToScene(pView->pos()).toPoint();

        mNewObject->SetX1(scenePos.x() + 100);
        mNewObject->SetX2(scenePos.x() + 200);

        mNewObject->SetY1(scenePos.y() + 100);
        mNewObject->SetY2(scenePos.y() + 100);

        mNewObject->SetPrevious(-1);
        mNewObject->SetNext(-1);

        mNewObject->CalculateLength();

        mArrowItem = mTab->MakeResizeableArrowItem(mNewObject.get());
    }

    SelectionSaver mSelectionSaver;
    bool mAdded = false;
    EditorTab* mTab = nullptr;
    std::unique_ptr<CollisionObject> mNewObject;
    ResizeableArrowItem* mArrowItem = nullptr;
};

void EditorTab::AddCollision()
{
    mUndoStack.push(new AddCollisionCommand(this));
}

void EditorTab::ConnectCollisions()
{
    if (!mScene->selectedItems().isEmpty())
    {
        std::vector<ResizeableArrowItem*> collisions;

        for (auto& selectedItem : mScene->selectedItems())
        {
            auto asResizableArrowItem = qgraphicsitem_cast<ResizeableArrowItem*>(selectedItem);
            if (asResizableArrowItem != nullptr)
            {
                collisions.push_back(asResizableArrowItem);
            }
        }

        std::vector<CollisionConnectData> collisionConnectData = CollisionConnectCommand::getConnectCollisionsChanges(collisions);

        if (!collisionConnectData.empty())
        {
            mUndoStack.push(new CollisionConnectCommand(collisionConnectData));
            mStatusBar->showMessage(tr("Connected collisions"));
        }
    }

}

int EditorTab::SnapX(bool enabled, int x)
{
    if (enabled)
    {
        if (mModel->Game() == GameType::eAo)
        {
            x = SnapToXGrid_AO(FP_FromInteger(1), x) + 13;
        }
        else
        {
            x = SnapToXGrid_AE(FP_FromInteger(1), x) + 13;
        }
    }
    return x;
}

int EditorTab::SnapY(bool enabled, int y)
{
    if (enabled)
    {
        const auto yGridSize = 20; // 260/13
        y = (y / yGridSize) * yGridSize;
    }
    return y;
}
