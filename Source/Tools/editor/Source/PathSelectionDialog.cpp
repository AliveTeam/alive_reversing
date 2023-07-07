#include "PathSelectionDialog.hpp"
#include "ui_PathSelectionDialog.h"
#include "relive_api.hpp"

PathSelectionDialog::PathSelectionDialog(QWidget *parent, ReliveAPI::EnumeratePathsResult& paths) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::PathSelectionDialog),
    mPaths(paths)
{
    ui->setupUi(this);

    for (int path : mPaths.paths)
    {
        auto item = new QListWidgetItem(QString::fromStdString(mPaths.pathBndName + "!" + std::to_string(path)));
        ui->listWidget->addItem(item);
    }
}

PathSelectionDialog::~PathSelectionDialog()
{
    delete ui;
}

void PathSelectionDialog::on_buttonBox_accepted()
{
    mSelectedPath = std::nullopt;

    QList<QListWidgetItem*> selection = ui->listWidget->selectedItems();
    if (!selection.empty())
    {
        const int idx = ui->listWidget->currentIndex().row();
        mSelectedPath = std::make_optional(mPaths.paths[idx]);
    }
}

void PathSelectionDialog::on_buttonBox_rejected()
{
    mSelectedPath = std::nullopt;
}

