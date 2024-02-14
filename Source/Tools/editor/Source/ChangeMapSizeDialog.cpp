#include "ChangeMapSizeDialog.hpp"
#include "ui_ChangeMapSizeDialog.h"
#include "EditorTab.hpp"
#include <QUndoCommand>
#include "EditorGraphicsScene.hpp"
#include "SelectionSaver.hpp"
#include "ResizeableRectItem.hpp"
#include "CameraGraphicsItem.hpp"
#include <QDebug>

struct RemovedCamera final
{
    // Item we've removed from the model
     Model::UP_Camera mCameraModel;

    // The graphics item for this camera
    CameraGraphicsItem* mCameraGraphicsItem = nullptr;

    // All of the graphics items for this cameras map object
    QList<ResizeableRectItem*> mGraphicsItemMapObjects;

    RemovedCamera(CameraGraphicsItem* pCameraGraphicsItem, EditorGraphicsScene* pScene)
        : mCameraGraphicsItem(pCameraGraphicsItem)
    {
        // Collect the map object graphics items for this camera
        mGraphicsItemMapObjects = pScene->MapObjectsForCamera(mCameraGraphicsItem);
    }

    ~RemovedCamera()
    {
        if (mCameraModel)
        {
            delete mCameraGraphicsItem;
            for (auto& mapObj : mGraphicsItemMapObjects)
            {
                delete mapObj;
            }
        }
    }

    void redo(Model& model, EditorGraphicsScene* pScene)
    {
        // Remove it from the model now we have the model item
        mCameraModel = model.RemoveCamera(mCameraGraphicsItem->GetCamera());

        // Remove graphics items from the scene
        pScene->removeItem(mCameraGraphicsItem);
        for (auto& mapObject : mGraphicsItemMapObjects)
        {
            pScene->removeItem(mapObject);
        }
    }

    void undo(Model& model, EditorGraphicsScene* pScene)
    {
        // Add camera back to the model
        model.AddCamera(std::move(mCameraModel));

        // Add graphics items to the scene
        pScene->addItem(mCameraGraphicsItem);
        for (auto& mapObject : mGraphicsItemMapObjects)
        {
            pScene->addItem(mapObject);
        }
    }
};

struct CamToEdit final
{
    int x = 0;
    int y = 0;
    bool mAdd = false;

    bool operator == (const CamToEdit& rhs) const
    {
        return x == rhs.x && y == rhs.y && mAdd == rhs.mAdd;
    }
};

static void Add(std::vector<CamToEdit>& edits, int x, int y, bool add)
{
    for (auto& edit : edits)
    {
        if (edit.x == x && edit.y == y)
        {
            edit.mAdd = add;
            return;
        }
    }
    edits.push_back(CamToEdit{ x, y, add });
}

static std::vector<CamToEdit> CalcMapChanges(int oldW, int newW, int oldH, int newH)
{
    // Calculate coords to remove or add cameras to
    const int maxX = std::max(oldW, newW);
    const int maxY = std::max(oldH, newH);

    const int minX = std::min(oldW, newW);
    const int minY = std::min(oldH, newH);

    std::vector<CamToEdit> edits;
    for (int x = 0; x < maxX; x++)
    {
        for (int y = 0; y < maxY; y++)
        {
            if (x >= oldW)
            {
                // add
                Add(edits, x, y, true);
            }
            else if (x >= newW)
            {
                // remove
                Add(edits, x, y, false);
            }

            if (y >= oldH)
            {
                // add
                Add(edits, x, y, true);
            }
            else if (y >= newH)
            {
                // remove
                Add(edits, x, y, false);
            }
        }
    }

    for (auto& edit : edits)
    {
        qDebug() << edit.x << ", " << edit.y << (edit.mAdd ? " remove" : " add");
    }

    return edits;
}

static bool Equal(const std::vector<CamToEdit>& lhs, const std::vector<CamToEdit>& rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }

    for (const auto& lhsItem : lhs)
    {
        bool found = false;
        for (const auto& rhsItem : rhs)
        {
            if (rhsItem == lhsItem)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }

    return true;
}

void Test_RemoveCoulmn()
{
    auto edits = CalcMapChanges(2, 1, 2, 2);
    std::vector<CamToEdit> expected = { CamToEdit{1, 0, false}, CamToEdit{1, 1, false} };
    if (!Equal(edits, expected))
    {
        abort();
    }
}

void Test_AddCoulmn()
{
    auto edits = CalcMapChanges(2, 3, 2, 2);
    std::vector<CamToEdit> expected = { CamToEdit{2, 0, true}, CamToEdit{2, 1, true} };
    if (!Equal(edits, expected))
    {
        abort();
    }
}

void Test_RemoveRow()
{
    auto edits = CalcMapChanges(2, 2, 2, 1);
    std::vector<CamToEdit> expected = { CamToEdit{0, 1, false}, CamToEdit{1, 1, false} };
    if (!Equal(edits, expected))
    {
        abort();
    }
}

void Test_AddRow()
{
    auto edits = CalcMapChanges(2, 2, 2, 3);
    std::vector<CamToEdit> expected = { CamToEdit{0, 2, true}, CamToEdit{1, 2, true} };
    if (!Equal(edits, expected))
    {
        abort();
    }
}

void Test_RemoveCoulmn_AddRow()
{
    auto edits = CalcMapChanges(2, 1, 2, 3);
    std::vector<CamToEdit> expected = { 
        CamToEdit{1, 0, false}, CamToEdit{1, 1, false},
        CamToEdit{0, 2, true}, CamToEdit{1, 2, true} };

    if (!Equal(edits, expected))
    {
        abort();
    }
}

void Test_AddCoulmn_AddRow()
{
    auto edits = CalcMapChanges(2, 3, 2, 3);
    std::vector<CamToEdit> expected = { 
        CamToEdit{2, 0, true}, CamToEdit{2, 1, true},
        CamToEdit{0, 2, true}, CamToEdit{1, 2, true}, 
        CamToEdit{2, 2, true} };
    if (!Equal(edits, expected))
    {
        abort();
    }
}

void Test_RemoveCoulmn_RemoveRow()
{
    auto edits = CalcMapChanges(2, 1, 2, 1);
    std::vector<CamToEdit> expected = { 
        CamToEdit{1, 0, false}, CamToEdit{1, 1, false},
        CamToEdit{0, 1, false}, };
    if (!Equal(edits, expected))
    {
        abort();
    }
}
void DoMapSizeTests()
{
    Test_RemoveCoulmn();
    Test_AddCoulmn();
    Test_RemoveRow();
    Test_AddRow();
    Test_RemoveCoulmn_AddRow();
    Test_AddCoulmn_AddRow();
    Test_RemoveCoulmn_RemoveRow();
}

struct AddedCamera final
{
    // New camera
    CameraGraphicsItem* mCameraGraphicsItem = nullptr;

    // New model item
    Model::UP_Camera mCameraModel;

    AddedCamera(EditorTab* pTab, int x, int y)
    {
        mCameraModel = std::make_unique<Model::Camera>();
        mCameraModel->mX = x;
        mCameraModel->mY = y;
        const auto& model = pTab->GetModel();
        mCameraGraphicsItem = pTab->MakeCameraGraphicsItem(mCameraModel.get(), model.CameraGridWidth() * x, y * model.CameraGridHeight(), model.CameraGridWidth(), model.CameraGridHeight());
    }

    ~AddedCamera()
    {
       // if (mCameraModel)
        {
          //  delete mCameraGraphicsItem;
        }
    }

    void undo(Model& model, EditorGraphicsScene* pScene)
    {
        //mCameraModel = model.RemoveCamera(mCameraGraphicsItem->GetCamera());
       // pScene->removeItem(mCameraGraphicsItem);
    }

    void redo(Model& model, EditorGraphicsScene* pScene)
    {
        model.AddCamera(std::move(mCameraModel));
        pScene->addItem(mCameraGraphicsItem);
    }
};

// TODO: Handle collision items that are now outside of the map rect
class ChangeMapSizeCommand final : public QUndoCommand
{
public:
    ChangeMapSizeCommand(EditorTab* pTab, int newXSize, int newYSize)
      : mSelectionSaver(pTab),
        mTab(pTab),
        mNewXSize(newXSize),
        mNewYSize(newYSize)
    {
        mOldXSize = mTab->GetModel().XSize();
        mOldYSize = mTab->GetModel().YSize();

        setText("Change map size from " + 
            QString::number(mOldXSize) + "x" + QString::number(mOldYSize) + " to " +
            QString::number(mNewXSize) + "x" + QString::number(mNewYSize));

        auto edits = CalcMapChanges(mOldXSize, mNewXSize, mOldYSize, mNewYSize);

        for (auto& edit : edits)
        {
            if (!edit.mAdd)
            {
                CameraGraphicsItem* pCam = mTab->GetScene().CameraAt(edit.x, edit.y);
                mRemovedCameras.emplace_back(std::make_unique<RemovedCamera>(pCam, &mTab->GetScene()));
            }
            else
            {
                mAddedCameras.emplace_back(std::make_unique<AddedCamera>(mTab, edit.x, edit.y));
            }
        }
    }

    void undo() override
    {
        mTab->GetModel().SetXSize(mOldXSize);
        mTab->GetModel().SetYSize(mOldYSize);

        // Fix scene rect
        mTab->GetScene().UpdateSceneRect();

        // Add back removed cameras and objects
        for (auto& removed : mRemovedCameras)
        {
            removed->undo(mTab->GetModel(), &mTab->GetScene());
        }

        // Remove newly added cameras
        for (auto& added : mAddedCameras)
        {
            added->undo(mTab->GetModel(), &mTab->GetScene());
        }

        mSelectionSaver.undo();
    }

    void redo() override
    {
        mSelectionSaver.redo();

        mTab->GetModel().SetXSize(mNewXSize);
        mTab->GetModel().SetYSize(mNewYSize);

        // Fix scene rect
        mTab->GetScene().UpdateSceneRect();

        // Remove cameras and objects
        for (auto& removed : mRemovedCameras)
        {
            removed->redo(mTab->GetModel(), &mTab->GetScene());
        }

        // Add new cameras
        for (auto& added : mAddedCameras)
        {
            added->redo(mTab->GetModel(), &mTab->GetScene());
        }
    }

private:
    EditorTab* mTab = nullptr;

    SelectionSaver mSelectionSaver;

    int mOldXSize = 0;
    int mOldYSize = 0;

    int mNewXSize = 0;
    int mNewYSize = 0;

    std::vector<std::unique_ptr<RemovedCamera>> mRemovedCameras;
    std::vector<std::unique_ptr<AddedCamera>> mAddedCameras;
};

ChangeMapSizeDialog::ChangeMapSizeDialog(QWidget *parent, EditorTab* pTab) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::ChangeMapSizeDialog),
    mTab(pTab)
{
    ui->setupUi(this);

    ui->spnXSize->setMinimum(1);
    ui->spnYSize->setMinimum(1);

    ui->spnXSize->setValue(mTab->GetModel().XSize());
    ui->spnYSize->setValue(mTab->GetModel().YSize());
}

ChangeMapSizeDialog::~ChangeMapSizeDialog()
{
    delete ui;
}

void ChangeMapSizeDialog::on_buttonBox_accepted()
{
    if (mTab->GetModel().XSize() != ui->spnXSize->value() ||
        mTab->GetModel().YSize() != ui->spnYSize->value())
    {
        mTab->AddCommand(new ChangeMapSizeCommand(mTab, ui->spnXSize->value(), ui->spnYSize->value()));
    }
}
