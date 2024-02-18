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

/*
class ObjectListItem final : public QListWidgetItem
{
public:
    ObjectListItem(const UP_ObjectStructure& obj)
        : QListWidgetItem(obj->mName.c_str()),
          mObj(obj)
    {

    }

    const UP_ObjectStructure& GetObjectStructure() const
    {
        return mObj;
    }

private:
    const UP_ObjectStructure& mObj;
};
*/
class AddNewObjectCommand : public QUndoCommand
{
public: /*
    AddNewObjectCommand(EditorTab* pTab, const UP_ObjectStructure& objStructure)
        : mTab(pTab),
        mObjStructure(objStructure),
        mSelectionSaver(pTab)
    {
        MakeNewObject();

        setText(QString("Add new object ") + objStructure->mName.c_str());
    }*/

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
        /*
        auto pNewMapObj = new MapObject();

        pNewMapObj->mObjectStructureType = mObjStructure->mName;
        for (auto& prop : mObjStructure->mEnumAndBasicTypeProperties)
        {
            auto foundType = mTab->GetModel().FindType(prop.mType);

            auto newProp = mTab->GetModel().MakeProperty(foundType, prop, mObjStructure.get());
            if (foundType.mEnum)
            {
                auto enumType = mTab->GetModel().FindEnum(prop.mType);
                newProp->mEnumValue = enumType->mValues[0];
            }

            pNewMapObj->mProperties.emplace_back(std::move(newProp));
        }
        
        pNewMapObj->SetXPos(viewPos.x());
        pNewMapObj->SetYPos(viewPos.y());
        pNewMapObj->SetWidth(24);
        pNewMapObj->SetHeight(24);

        mNewItem = mTab->MakeResizeableRectItem(pNewMapObj);
        */
    }

    SelectionSaver mSelectionSaver;
    EditorCamera* mCamera = nullptr;
    bool mAdded = false;
    ResizeableRectItem* mNewItem = nullptr;
    EditorTab* mTab;
    //const UP_ObjectStructure& mObjStructure;
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
    /*
    const auto& structures = mTab->GetModel().GetObjectStructures();
    for (const auto& s : structures)
    {
        if (filter.isEmpty() || QString(s->mName.c_str()).toLower().contains(filter.toLower()))
        {
            ui->lstObjects->addItem(new ObjectListItem(s));
        }
    }*/
}

void AddObjectDialog::on_buttonBox_accepted()
{ /*
    if (!ui->lstObjects->selectedItems().isEmpty())
    {
        auto pItem = static_cast<ObjectListItem*>(ui->lstObjects->selectedItems().at(0));
        const UP_ObjectStructure& objStructure = pItem->GetObjectStructure();
        mTab->AddCommand(new AddNewObjectCommand(mTab, objStructure));
    }*/
}

