#include "PathDataEditorDialog.hpp"
#include "ui_PathDataEditorDialog.h"
#include "EditorTab.hpp"
#include <QUndoCommand>

struct UndoRedoPathData final
{
    int mudsInLvl;
    int abeXPos;
    int abeYPos;
    int badEndingMuds;
    int goodEndingMuds;
    int totalMuds;
};

class EditPathDataCommand final : public QUndoCommand
{
public:
    EditPathDataCommand(Model& model, const UndoRedoPathData& oldPd, const UndoRedoPathData& newPd)
        : mModel(model),
        mOldPd(oldPd),
        mNewPd(newPd)
    {
        setText("Edit path data");
    }

    void undo() override
    {
        MapInfo& info = mModel.GetMapInfo();
        SetPd(info, mOldPd);
    }

    void redo() override
    {
        MapInfo& info = mModel.GetMapInfo();
        SetPd(info, mNewPd);
    }

private:
    void SetPd(MapInfo& info, const UndoRedoPathData& pd)
    {
        info.mAbeStartXPos = pd.abeXPos;
        info.mAbeStartYPos = pd.abeYPos;
        info.mBadEndingMuds = pd.badEndingMuds;
        info.mGoodEndingMuds = pd.goodEndingMuds;
        info.mTotalMuds = pd.totalMuds;
        info.mNumMudsInPath = pd.mudsInLvl;
    }

    Model& mModel;
    const UndoRedoPathData mOldPd;
    const UndoRedoPathData mNewPd;
};

PathDataEditorDialog::PathDataEditorDialog(QWidget *parent, EditorTab* pTab) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::PathDataEditorDialog),
    mTab(pTab)
{
    ui->setupUi(this);

    ui->spnNumMudsInLvl->setValue(mTab->GetModel().GetMapInfo().mNumMudsInPath);
    ui->spnAbeStartXPos->setValue(mTab->GetModel().GetMapInfo().mAbeStartXPos);
    ui->spnAbeStartYPos->setValue(mTab->GetModel().GetMapInfo().mAbeStartYPos);

    ui->spnBadEndingMuds->setValue(mTab->GetModel().GetMapInfo().mBadEndingMuds);
    ui->spnGoodEndingMuds->setValue(mTab->GetModel().GetMapInfo().mGoodEndingMuds);
    ui->spnTotalMudsInGame->setValue(mTab->GetModel().GetMapInfo().mTotalMuds);
}

PathDataEditorDialog::~PathDataEditorDialog()
{
    delete ui;
}


void PathDataEditorDialog::on_buttonBox_accepted()
{
    UndoRedoPathData pd = {};
    pd.mudsInLvl = ui->spnNumMudsInLvl->value();
    pd.abeXPos = ui->spnAbeStartXPos->value();
    pd.abeYPos = ui->spnAbeStartYPos->value();
    pd.badEndingMuds = ui->spnBadEndingMuds->value();
    pd.goodEndingMuds = ui->spnGoodEndingMuds->value();
    pd.totalMuds = ui->spnTotalMudsInGame->value();
    
    MapInfo& info = mTab->GetModel().GetMapInfo();

    if (info.mNumMudsInPath != pd.mudsInLvl ||
        info.mAbeStartXPos != pd.abeXPos ||
        info.mAbeStartYPos != pd.abeYPos ||
        info.mBadEndingMuds != pd.badEndingMuds ||
        info.mGoodEndingMuds != pd.goodEndingMuds ||
        info.mTotalMuds != pd.totalMuds)
    {
        UndoRedoPathData oldPd = {};
        oldPd.mudsInLvl = info.mNumMudsInPath;
        oldPd.abeXPos = info.mAbeStartXPos;
        oldPd.abeYPos = info.mAbeStartYPos;
        oldPd.badEndingMuds = info.mBadEndingMuds;
        oldPd.goodEndingMuds = info.mGoodEndingMuds;
        oldPd.totalMuds = info.mTotalMuds;

        mTab->AddCommand(new EditPathDataCommand(mTab->GetModel(), oldPd, pd));
    }
}


void PathDataEditorDialog::on_buttonBox_rejected()
{
    // Do nothing
}
