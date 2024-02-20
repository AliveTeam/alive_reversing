#include "AddObjectDialog.hpp"
#include "ui_AddObjectDialog.h"
#include "EditorTab.hpp"
#include "EditorGraphicsScene.hpp"
#include <QUndoCommand>
#include <QGraphicsItem>
#include "ResizeableRectItem.hpp"
#include "SelectionSaver.hpp"
#include "Model.hpp"
#include "EditorTab.hpp"
#include "../../relive_api/TlvsRelive.hpp"
#include "ReflectedEnumProperties.hpp"

class ObjectListItem final : public QListWidgetItem
{
public:
    ObjectListItem(QString objName, MapObjectBase::TEditorNewFunc objFunc)
        : QListWidgetItem(objName), mNewFunc(objFunc)
    {

    }

    const MapObjectBase::TEditorNewFunc NewFunc() const
    {
        return mNewFunc;
    }

private:
    MapObjectBase::TEditorNewFunc mNewFunc = nullptr;
};

class AddNewObjectCommand : public QUndoCommand
{
public:
    AddNewObjectCommand(EditorTab* pTab, QString objectName, MapObjectBase::TEditorNewFunc newFunc)
      : mSelectionSaver(pTab),
        mTab(pTab),
        mNewFunc(newFunc)
    {
        MakeNewObject();

        setText(QString("Add new object ") + objectName);
    }

    ~AddNewObjectCommand()
    {
        if (!mAdded)
        {
            delete mNewItem->GetMapObject(); // not owned by the model as its removed during undo
            delete mNewItem;
        }
    }

    void undo() override
    {
        // Remove from model
        for (auto it = mCamera->mMapObjects.begin(); it != mCamera->mMapObjects.end(); it++)
        {
            if (it->get() == mNewItem->GetMapObject())
            {
                // Do not delete
                it->release();

                mCamera->mMapObjects.erase(it);

                break;
            }
        }

        // Remove from scene
        mTab->GetScene().removeItem(mNewItem);
        mAdded = false;

        mSelectionSaver.undo();
    }

    void redo() override
    {
        // Add to model
        mCamera->mMapObjects.emplace_back(std::unique_ptr<MapObjectBase>(mNewItem->GetMapObject()));

        // Add to scene
        mTab->GetScene().addItem(mNewItem);

        // Set the new item as the only thing selected
        mTab->GetScene().clearSelection();
        mNewItem->setSelected(true);

        mSelectionSaver.redo();

        mAdded = true;
    }

private:
    void MakeNewObject()
    {
        // Find the camera to add the object to
        QGraphicsView* pView = mTab->GetScene().views().at(0);
        QPoint viewPos = pView->mapToScene(pView->rect().center()).toPoint();

        int camX = viewPos.x() / mTab->GetModel().CameraGridWidth();
        if (camX < 0)
        {
            camX = 0;
        }
        if (camX > mTab->GetModel().CameraGridWidth())
        {
            camX = mTab->GetModel().CameraGridWidth();
        }

        int camY = viewPos.y() / mTab->GetModel().CameraGridHeight();
        if (camY < 0)
        {
            camY = 0;
        }
        if (camY > mTab->GetModel().CameraGridHeight())
        {
            camY = mTab->GetModel().CameraGridHeight();
        }

        mCamera = mTab->GetModel().CameraAt(camX, camY);

        auto pNewMapObj = mNewFunc();
        pNewMapObj->SetXPos(viewPos.x());
        pNewMapObj->SetYPos(viewPos.y());
        pNewMapObj->SetWidth(24);
        pNewMapObj->SetHeight(24);

        // NOTE: Inner pNewMapObj deleted in dtor if not given to the model
        mNewItem = mTab->MakeResizeableRectItem(pNewMapObj.release());

    }

    SelectionSaver mSelectionSaver;
    EditorCamera* mCamera = nullptr;
    bool mAdded = false;
    ResizeableRectItem* mNewItem = nullptr;
    EditorTab* mTab = nullptr;
    MapObjectBase::TEditorNewFunc mNewFunc = nullptr;
};

AddObjectDialog::AddObjectDialog(QWidget *parent, EditorTab* pTab) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::AddObjectDialog),
    mTab(pTab)
{
    ui->setupUi(this);
    ui->txtSearch->setFocus();
    PopulateListFiltered("");
}

AddObjectDialog::~AddObjectDialog()
{
    delete ui;
}

void AddObjectDialog::on_txtSearch_textChanged(const QString &arg1)
{
    PopulateListFiltered(arg1);
}

void AddObjectDialog::PopulateListFiltered(QString filter)
{
    ui->lstObjects->clear();

    const auto& factoryEntries = MapObjectBase::GetEditorFactoryRegistry();
    const auto& tlvEnumValues = EnumReflector<ReliveTypes>::mArray;
    for (auto enumIterator = tlvEnumValues.begin(); enumIterator != tlvEnumValues.end(); enumIterator++)
    {
        auto factoryEntry = factoryEntries.find(enumIterator->mValue);
        if (factoryEntry == std::end(factoryEntries))
        {
            continue;
        }
        
        QString name = enumIterator->mName;
        if (filter.isEmpty() || name.toLower().contains(filter.toLower()))
        {
            ui->lstObjects->addItem(new ObjectListItem(name, factoryEntry->second.mNewFunc));
        }
    }
}

void AddObjectDialog::on_buttonBox_accepted()
{
    if (!ui->lstObjects->selectedItems().isEmpty())
    {
        auto pItem = static_cast<ObjectListItem*>(ui->lstObjects->selectedItems().at(0));
        mTab->AddCommand(new AddNewObjectCommand(mTab, pItem->text(), pItem->NewFunc()));
    }
}

