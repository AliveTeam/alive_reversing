#include "CameraManager.hpp"
#include "ui_CameraManager.h"
#include "EditorTab.hpp"
#include "EditorGraphicsScene.hpp"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QGraphicsItem>
#include "CameraGraphicsItem.hpp"
#include "SelectionSaver.hpp"
#include "ResizeableRectItem.hpp"
#include "Model.hpp"

static QPixmap Base64ToPixmap(const std::string& s)
{
    QPixmap tmp;
    tmp.loadFromData(QByteArray::fromBase64(QByteArray(s.c_str(), static_cast<int>(s.length()))));
    return tmp;
}

static std::string PixmapToBase64PngString(QPixmap img)
{
    QPixmap pixmap;
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");
    buffer.close();

    return bytes.toBase64().toStdString();
}

static std::string CameraNameFromId(Model& model, int camId)
{
    /*
    std::string lvl = model.GetMapInfo().mPathBnd.substr(0, 2);
    std::string path = std::to_string(model.GetMapInfo().mPathId);
    if (path.length() != 2)
    {
        path = "0" + path;
    }

    std::string strCamId = std::to_string(camId);
    if (strCamId.length() != 2)
    {
        strCamId = "0" + strCamId;
    }

    return lvl + "P" + path + "C" + strCamId;
    */
    return "todo";
}

static int CamIdFromCamName(const std::string camName)
{
    if (camName.empty())
    {
        return -1;
    }
    std::string strCamId = camName.substr(6, 2);

    return QString(strCamId.c_str()).toInt();
}

enum TabImageIdx
{
    Main = 0,
    Foreground = 1,
    Background = 2,
    ForegroundWell = 3,
    BackgroundWell = 4,
};

class NewCameraCommand final : public QUndoCommand
{
public:
    NewCameraCommand(CameraGraphicsItem* pCameraGraphicsItem, QPixmap newImage, EditorTab* pEditorTab, const std::string& newCamName, int newCamId)
        : mItem(pCameraGraphicsItem), mCamImage(newImage), mTab(pEditorTab), mNewCamName(newCamName), mNewCamId(newCamId)
    {
        /*
        int pathIdShifted = mTab->GetModel().GetMapInfo().mPathId * 100;
        mNewCamId += pathIdShifted;
        QString posStr = QString::number(pCameraGraphicsItem->GetCamera()->mX) + "," + QString::number(pCameraGraphicsItem->GetCamera()->mY);

        setText("Create new camera at " + posStr);
        */
    }
    
    void redo() override
    {
        /*
        mItem->GetCamera()->mId = mNewCamId;
        mItem->GetCamera()->mName = mNewCamName;

        mItem->SetImage(mCamImage);
        mItem->GetCamera()->mCameraImageandLayers.mCameraImage = PixmapToBase64PngString(mCamImage);

        mTab->GetScene().update();

        // Update camera manager UI if open
        CameraManager* pMgr = mTab->GetCameraManagerDialog();
        if (pMgr)
        {
            pMgr->OnCameraIdChanged(mItem->GetCamera());
        }
        */
    }

    void undo() override
    {
        /*
        mItem->GetCamera()->mId = 0;
        mItem->GetCamera()->mName.clear();

        mItem->SetImage(QPixmap());
        mItem->GetCamera()->mCameraImageandLayers.mCameraImage.clear();

        mTab->GetScene().update();

        // Update camera manager UI if open
        CameraManager* pMgr = mTab->GetCameraManagerDialog();
        if (pMgr)
        {
            pMgr->OnCameraIdChanged(mItem->GetCamera());
        }
        */
    }

private:
    CameraGraphicsItem* mItem = nullptr;
    QPixmap mCamImage;
    EditorTab* mTab = nullptr;
    std::string mNewCamName;
    int mNewCamId = 0;
};

class ChangeCameraImageCommand final : public QUndoCommand
{
public:
    ChangeCameraImageCommand(CameraGraphicsItem* pCameraGraphicsItem, QPixmap newImage, TabImageIdx imgIdx, EditorTab* pEditorTab)
        : mCameraGraphicsItem(pCameraGraphicsItem), mEditorTab(pEditorTab),  mNewImage(newImage), mImgIdx(imgIdx)
    {
        /*
        // todo: set correctly
        QString posStr = QString::number(mCameraGraphicsItem->GetCamera()->mX) + "," + QString::number(mCameraGraphicsItem->GetCamera()->mY);


        switch (mImgIdx)
        {
        case Main:
            mOldImage = pCameraGraphicsItem->GetImage();
            setText("Change camera image at " + posStr);
            break;

        case Foreground:
            mOldImage = Base64ToPixmap(mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mForegroundLayer);
            setText("Change camera foreground image at " + posStr);
            break;

        case Background:
            mOldImage = Base64ToPixmap(mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mBackgroundLayer);
            setText("Change camera background image at " + posStr);
            break;

        case ForegroundWell:
            mOldImage = Base64ToPixmap(mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mForegroundWellLayer);
            setText("Change camera foreground well image at " + posStr);
            break;

        case BackgroundWell:
            mOldImage = Base64ToPixmap(mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mBackgroundWellLayer);
            setText("Change camera background well image at " + posStr);
            break;
        }
        */
    }

    void undo() override
    {
        UpdateImage(mOldImage);
    }

    void redo() override
    {
        UpdateImage(mNewImage);
    }

private:
    void UpdateImage(QPixmap img)
    {
        /*
        switch (mImgIdx)
        {
        case Main:
            mCameraGraphicsItem->SetImage(img);
            mEditorTab->GetScene().invalidate();
            mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mCameraImage = PixmapToBase64PngString(img);
            break;

        case Foreground:
            mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mForegroundLayer = PixmapToBase64PngString(img);
            break;

        case Background:
            mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mBackgroundLayer = PixmapToBase64PngString(img);
            break;

        case ForegroundWell:
            mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mForegroundWellLayer = PixmapToBase64PngString(img);
            break;

        case BackgroundWell:
            mCameraGraphicsItem->GetCamera()->mCameraImageandLayers.mBackgroundWellLayer = PixmapToBase64PngString(img);
            break;
        };
        */
    }

    CameraGraphicsItem* mCameraGraphicsItem = nullptr;
    EditorTab* mEditorTab;

    QPixmap mNewImage;
    QPixmap mOldImage;

    TabImageIdx mImgIdx = {};
};

class DeleteCameraCommand final : public QUndoCommand
{
    Q_DECLARE_TR_FUNCTIONS(DeleteCameraCommand)

public:
    DeleteCameraCommand(EditorTab* pTab, CameraGraphicsItem* pItem)
        : mSelectionSaver(pTab), mTab(pTab), mCameraOriginal(pItem)
    {
        setText(tr("Delete camera at %1,%2").arg(pItem->GetCamera()->mX).arg(pItem->GetCamera()->mY));

        mEmptyCameraModel = std::make_unique<EditorCamera>();
        mEmptyCameraModel->mX = pItem->GetCamera()->mX;
        mEmptyCameraModel->mY = pItem->GetCamera()->mY;

        mEmptyCamera = mTab->MakeCameraGraphicsItem(
            mEmptyCameraModel.get(),
            mTab->GetModel().CameraGridWidth() * mEmptyCameraModel->mX,
            mTab->GetModel().CameraGridHeight() * mEmptyCameraModel->mY,
            mTab->GetModel().CameraGridWidth(),
            mTab->GetModel().CameraGridHeight());

        mGraphicsItemMapObjects = mTab->GetScene().MapObjectsForCamera(mCameraOriginal);
    }

    ~DeleteCameraCommand()
    {
        if (mAdded)
        {
            delete mEmptyCamera;
        }
        else
        {
            delete mCameraOriginal;

            // Owned by the scene, not added so we own it
            for (auto& item : mGraphicsItemMapObjects)
            {
                delete item;
            }
        }
    }

    void undo() override
    {
        // Remove "blank" graphics item
        mTab->GetScene().removeItem(mEmptyCamera);
        mEmptyCameraModel = mTab->GetModel().RemoveCamera(mEmptyCamera->GetCamera());

        // Move empty camera map objects to original camera
        mCameraOriginalModel->mMapObjects = std::move(mEmptyCameraModel->mMapObjects);

        // Add back the original camera
        mTab->GetModel().AddCamera(std::move(mCameraOriginalModel));
        mTab->GetScene().addItem(mCameraOriginal);

        // Add map objects back to the graphics scene
        for (auto& item : mGraphicsItemMapObjects)
        {
            mTab->GetScene().addItem(item);
        }

        // Update camera manager UI if open
        CameraManager* pMgr = mTab->GetCameraManagerDialog();
        if (pMgr)
        {
            pMgr->OnCameraSwapped(mEmptyCamera->GetCamera(), mCameraOriginal->GetCamera());
        }
        mSelectionSaver.undo();

        mAdded = true;
    }

    void redo() override
    {
        // Remove original camera
        mCameraOriginalModel = mTab->GetModel().RemoveCamera(mCameraOriginal->GetCamera());
        mTab->GetScene().removeItem(mCameraOriginal);

        // Move original map objects to blank camera
        mEmptyCameraModel->mMapObjects = std::move(mCameraOriginalModel->mMapObjects);

        // Remove map object graphics items
        for (auto& item : mGraphicsItemMapObjects)
        {
            mTab->GetScene().removeItem(item);
        }
        // Add "blank" camera
        mTab->GetScene().addItem(mEmptyCamera);
        mTab->GetModel().AddCamera(std::move(mEmptyCameraModel));

        // Update camera manager UI if open
        CameraManager* pMgr = mTab->GetCameraManagerDialog();
        if (pMgr)
        {
            pMgr->OnCameraSwapped(mCameraOriginal->GetCamera(), mEmptyCamera->GetCamera());
        }

        mSelectionSaver.redo();

        mAdded = false;
    }

private:

    QList<ResizeableRectItem*> mGraphicsItemMapObjects;

    SelectionSaver mSelectionSaver;
    EditorTab* mTab = nullptr;
    CameraGraphicsItem* mCameraOriginal = nullptr;
    std::unique_ptr<EditorCamera> mCameraOriginalModel;

    CameraGraphicsItem* mEmptyCamera = nullptr;
    std::unique_ptr<EditorCamera> mEmptyCameraModel;

    bool mAdded = false;
};

class CameraListItem final : public QListWidgetItem
{
public:
    CameraListItem(QListWidget* pParent, EditorCamera* pCamera)
        : QListWidgetItem(pParent), mCamera(pCamera)
    {
        SetLabel();
    }

    const EditorCamera* GetCamera() const
    {
        return mCamera;
    }

    void SetCamera(EditorCamera* pNew)
    {
        mCamera = pNew;
        SetLabel();
    }

    void SetLabel()
    {
        QString camPos = QString::number(mCamera->mX) + "," + QString::number(mCamera->mY);

        if (!mCamera->mName.empty())
        {
            setText(QString(mCamera->mName.c_str()) + " @ " + camPos);
        }
        else
        {
            setText(camPos);
        }
    }

private:
    EditorCamera* mCamera = nullptr;
};

CameraManager::CameraManager(QWidget *parent, EditorTab* pParentTab, const QPoint* openedPos) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::CameraManager),
    mTab(pParentTab)
{
    ui->setupUi(this);
    
    const Model& model = mTab->GetModel();

    const auto& cameras = model.GetCameras();
    for (const auto& cam : cameras)
    {
        ui->lstCameras->addItem(new CameraListItem(ui->lstCameras, cam.get()));
    }

    if (openedPos)
    {
        // work out which camera x,y was clicked on
        const int camX = openedPos->x() / model.CameraGridWidth();
        const int camY = openedPos->y() / model.CameraGridHeight();
        qDebug() << "Looking for a camera at " << camX << " , " << camY;

        const auto pCamera = mTab->GetModel().CameraAt(camX, camY);
        if (pCamera)
        {
            qDebug() << "Got a camera at " << camX << " , " << camY << " " << pCamera->mName.c_str();
            for (int i = 0; i < ui->lstCameras->count(); i++)
            {
                auto pItem = static_cast<CameraListItem*>(ui->lstCameras->item(i));
                if (pCamera == pItem->GetCamera())
                {
                    ui->lstCameras->clearSelection();
                    pItem->setSelected(true);
                    break;
                }
            }
        }
    }
}

CameraManager::~CameraManager()
{
    delete ui;
}

void CameraManager::OnCameraSwapped(EditorCamera* pOld, EditorCamera* pNew)
{
    for (int i = 0; i < ui->lstCameras->count(); i++)
    {
        auto pItem = static_cast<CameraListItem*>(ui->lstCameras->item(i));
        if (pItem->GetCamera() == pOld)
        {
            pItem->SetCamera(pNew);
            CameraGraphicsItem* pCameraGraphicsItem = CameraGraphicsItemByModelPtr(pItem->GetCamera());
            UpdateTabImages(pCameraGraphicsItem);
            break;
        }
    }
}

void CameraManager::OnCameraIdChanged(EditorCamera* pCam)
{
    for (int i = 0; i < ui->lstCameras->count(); i++)
    {
        auto pItem = static_cast<CameraListItem*>(ui->lstCameras->item(i));
        if (pItem->GetCamera() == pCam)
        {
            pItem->SetLabel();
            break;
        }
    }
}

void CameraManager::CreateCamera(bool dropEvent, QPixmap img)
{
    auto pItem = static_cast<CameraListItem*>(ui->lstCameras->selectedItems()[0]);
    CameraGraphicsItem* pCameraGraphicsItem = CameraGraphicsItemByModelPtr(pItem->GetCamera());

    if (img.isNull())
    {
        QMessageBox::critical(this, "Error", "Failed to load image");
        return;
    }

    if (img.width() != 640 || img.height() != 240)
    {
        img = img.scaled(640, 240);
        if (img.isNull())
        {
            QMessageBox::critical(this, "Error", "Failed to resize image");
            return;
        }
    }

    if (!pItem->GetCamera()->mName.empty())
    {
        // Update image of existing camera
        auto index = dropEvent ? TabImageIdx::Main : static_cast<TabImageIdx>(ui->tabWidget->currentIndex());
        mTab->AddCommand(new ChangeCameraImageCommand(pCameraGraphicsItem, img, index, mTab));
        UpdateTabImages(pCameraGraphicsItem);
    }
    else
    {
        // Create new camera
        const int camId = NextFreeCamId();
        if (camId == -1)
        {
            QMessageBox::critical(this, "Error", "No more free camera Ids (only 0-99 is valid)");
            return;
        }

        if (ui->tabWidget->currentIndex() != TabImageIdx::Main)
        {
            QMessageBox::critical(this, "Error", "You need to set the main image first when creating a new camera");
            return;
        }

        const std::string newCamName = CameraNameFromId(mTab->GetModel(), camId);
        mTab->AddCommand(new NewCameraCommand(pCameraGraphicsItem, img, mTab, newCamName, camId));

        if (!dropEvent)
        {
            UpdateTabImages(pCameraGraphicsItem);
        }
    }
}

void CameraManager::on_btnExportImage_clicked()
{
    /*
    if (!ui->lstCameras->selectedItems().empty())
    {
        auto pItem = static_cast<CameraListItem*>(ui->lstCameras->selectedItems()[0]);
        if (pItem->GetCamera()->mName.empty())
        {
            QMessageBox::critical(this, "Error", "The selected camera has no image");
            return;
        }

        const auto tabIndex = static_cast<TabImageIdx>(ui->tabWidget->currentIndex());
        QByteArray camName = QString::fromStdString(pItem->GetCamera()->mName).toLocal8Bit();
        QPixmap camToExport;
        switch (tabIndex)
        {
            case TabImageIdx::Main:
                camToExport = Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mCameraImage);
                break;

            case TabImageIdx::Foreground:
                camToExport = Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mForegroundLayer);
                camName.append("_fg");
                break;

            case TabImageIdx::Background:
                camToExport = Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mBackgroundLayer);
                camName.append("_bg");
                break;

            case TabImageIdx::ForegroundWell:
                camToExport = Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mForegroundWellLayer);
                camName.append("_fg_well");
                break;

            case TabImageIdx::BackgroundWell:
                camToExport = Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mBackgroundWellLayer);
                camName.append("_bg_well");
                break;
        }

        if (camToExport.isNull())
        {
            QMessageBox::critical(this, "Error", "The selected camera tab has no image");
            return;
        }

        QString cameraSaveFileName = QFileDialog::getSaveFileName(this, tr("Export camera image"), camName, tr("PNG Files (*.png);;All Files (*)"));
        if (cameraSaveFileName.isEmpty())
        {
            // They didn't want to save it
            return;
        }

        // Append .png file ext if not specified
        if (!cameraSaveFileName.endsWith(".png", Qt::CaseInsensitive))
        {
            cameraSaveFileName += ".png";
        }

        if (ui->radioButton_640x480->isChecked())
        {
            camToExport = camToExport.scaled(640, 480);
            camToExport.save(cameraSaveFileName);
        }
        else if (ui->radioButton_640x240->isChecked())
        {
            camToExport.save(cameraSaveFileName);
        }
    }*/
}

void CameraManager::on_btnSelectImage_clicked()
{
    if (!ui->lstCameras->selectedItems().empty())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open level"), "", tr("PNG image files (*.png);;"));
        if (!fileName.isEmpty())
        {
            CreateCamera(false, QPixmap(fileName));
        }
    }
}

CameraGraphicsItem* CameraManager::CameraGraphicsItemByPos(const QPoint& pos)
{
    QList<QGraphicsItem*> itemsAtPos = mTab->GetScene().items(pos);
    CameraGraphicsItem* pCameraGraphicsItem = nullptr;
    for (int i = 0; i < itemsAtPos.count(); i++)
    {
        pCameraGraphicsItem = qgraphicsitem_cast<CameraGraphicsItem*>(itemsAtPos.at(i));
        if (pCameraGraphicsItem)
        {
            break;
        }
    }
    return pCameraGraphicsItem;
}

CameraGraphicsItem* CameraManager::CameraGraphicsItemByModelPtr(const EditorCamera* cam)
{
    QList<QGraphicsItem*> itemsAtPos = mTab->GetScene().items();
    for (int i = 0; i < itemsAtPos.count(); i++)
    {
        CameraGraphicsItem* pCameraGraphicsItem = qgraphicsitem_cast<CameraGraphicsItem*>(itemsAtPos.at(i));
        if (pCameraGraphicsItem)
        {
            if (pCameraGraphicsItem->GetCamera() == cam)
            {
                return pCameraGraphicsItem;
            }
        }
    }
    return nullptr;
}

void CameraManager::on_btnDeleteImage_clicked()
{
    if (!ui->lstCameras->selectedItems().empty())
    {
        auto pItem = static_cast<CameraListItem*>(ui->lstCameras->selectedItems()[0]);
        CameraGraphicsItem* pCameraGraphicsItem = CameraGraphicsItemByModelPtr(pItem->GetCamera());
        if (!pItem->GetCamera()->mName.empty())
        {
            // Don't allow removing of the main camera image, because that makes no sense
            if (ui->tabWidget->currentIndex() != 0)
            {
                mTab->AddCommand(new ChangeCameraImageCommand(pCameraGraphicsItem, QPixmap(), static_cast<TabImageIdx>(ui->tabWidget->currentIndex()), mTab));
                UpdateTabImages(pCameraGraphicsItem);
            }
            else
            {
                on_btnDeleteCamera_clicked();
            }
        }
    }
}

class ChangeCameraIdCommand final : public QUndoCommand
{
    Q_DECLARE_TR_FUNCTIONS(ChangeCameraIdCommand)
public:
    ChangeCameraIdCommand(EditorTab* pTab, CameraGraphicsItem* pItem, int oldId, int newId)
        : mTab(pTab), mItem(pItem), mOldId(oldId), mNewId(newId)
    {
        setText(tr("Change camera id from %1 to %2").arg(oldId).arg(newId));
        /*
        const int pathIdShifted = mTab->GetModel().GetMapInfo().mPathId * 100;
        mOldId += pathIdShifted;
        mNewId += pathIdShifted;
        */
    }

    void redo() override
    {
        /*
        mItem->GetCamera()->mId = mNewId;
        int pathIdShifted = mTab->GetModel().GetMapInfo().mPathId * 100;
        const int id = mNewId - pathIdShifted;
        mItem->GetCamera()->mName = CameraNameFromId(mTab->GetModel(), id);
        
        mTab->GetScene().update();

        // Update camera manager UI if open
        CameraManager* pMgr = mTab->GetCameraManagerDialog();
        if (pMgr)
        {
            pMgr->OnCameraIdChanged(mItem->GetCamera());
        }*/
    }

    void undo() override
    {
        /*
        mItem->GetCamera()->mId = mOldId;
        int pathIdShifted = mTab->GetModel().GetMapInfo().mPathId * 100;
        const int id = mOldId - pathIdShifted;
        mItem->GetCamera()->mName = CameraNameFromId(mTab->GetModel(), id);

        mTab->GetScene().update();

        // Update camera manager UI if open
        CameraManager* pMgr = mTab->GetCameraManagerDialog();
        if (pMgr)
        {
            pMgr->OnCameraIdChanged(mItem->GetCamera());
        }
        */
        
    }

private:
    EditorTab* mTab = nullptr;
    CameraGraphicsItem* mItem = nullptr;
    int mOldId = 0;
    int mNewId = 0;
};

int CameraManager::NextFreeCamId()
{
    for (int i = 0; i < 99; i++)
    {
        bool used = false;
        for (int j = 0; j < ui->lstCameras->count(); j++)
        {
            auto pItem = static_cast<CameraListItem*>(ui->lstCameras->item(j));
            if (CamIdFromCamName(pItem->GetCamera()->mName) == i)
            {
                used = true;
                break;
            }
        }
        if (!used)
        {
            return i;
        }
    }
    return -1;
}

void CameraManager::on_btnSetCameraId_clicked()
{
    if (!ui->lstCameras->selectedItems().empty())
    {
        auto pItem = static_cast<CameraListItem*>(ui->lstCameras->selectedItems()[0]);
        CameraGraphicsItem* pCameraGraphicsItem = CameraGraphicsItemByModelPtr(pItem->GetCamera());
        if (!pItem->GetCamera()->mName.empty())
        {
            const int newCamId = ui->spnCameraId->value();
            const int oldCamId = CamIdFromCamName(pItem->GetCamera()->mName);
            if (oldCamId != newCamId)
            {
                for (int i = 0; i < ui->lstCameras->count(); i++)
                {
                    auto pItem = static_cast<CameraListItem*>(ui->lstCameras->item(i));
                    if (CamIdFromCamName(pItem->GetCamera()->mName) == newCamId)
                    {
                        QMessageBox::warning(this, "Error", tr("Camera Id already in use"));
                        return;
                    }
                }
                mTab->AddCommand(new ChangeCameraIdCommand(mTab, pCameraGraphicsItem, oldCamId, newCamId));
            }
        }
    }
}

void CameraManager::SetTabImage(int idx, QPixmap img)
{
    QWidget* pWidget = ui->tabWidget->widget(idx);
    QList<QLabel*> allTextEdits = pWidget->findChildren<QLabel*>();
    if (!allTextEdits.isEmpty())
    {
     
        allTextEdits.at(0)->setPixmap(img);
    }
}

void CameraManager::UpdateTabImages(CameraGraphicsItem* pItem)
{
    /*
    SetTabImage(TabImageIdx::Main, pItem->GetImage());
    SetTabImage(TabImageIdx::Foreground, Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mForegroundLayer));
    SetTabImage(TabImageIdx::Background, Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mBackgroundLayer));
    SetTabImage(TabImageIdx::ForegroundWell, Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mForegroundWellLayer));
    SetTabImage(TabImageIdx::BackgroundWell, Base64ToPixmap(pItem->GetCamera()->mCameraImageandLayers.mBackgroundWellLayer));
    */
}

void CameraManager::on_btnDeleteCamera_clicked()
{
    if (!ui->lstCameras->selectedItems().isEmpty())
    {
        auto pItem = static_cast<CameraListItem*>(ui->lstCameras->selectedItems()[0]);
        if (!pItem->GetCamera()->mName.empty())
        {
            CameraGraphicsItem* pCameraGraphicsItem = CameraGraphicsItemByModelPtr(pItem->GetCamera());
            mTab->AddCommand(new DeleteCameraCommand(mTab, pCameraGraphicsItem));
        }
    }
}

void CameraManager::on_lstCameras_itemSelectionChanged()
{
    if (!ui->lstCameras->selectedItems().isEmpty())
    {
        CameraListItem* pItem = static_cast<CameraListItem*>(ui->lstCameras->selectedItems()[0]);
        ui->stackedWidget->setCurrentIndex(1);

        CameraGraphicsItem* pCameraGraphicsItem = CameraGraphicsItemByModelPtr(pItem->GetCamera());
        UpdateTabImages(pCameraGraphicsItem);
    }
}
