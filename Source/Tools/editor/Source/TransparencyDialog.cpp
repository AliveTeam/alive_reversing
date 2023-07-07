#include "TransparencyDialog.hpp"
#include "ui_TransparencyDialog.h"
#include "EditorTab.hpp"

TransparencyDialog::TransparencyDialog(EditorTab* pTab, QWidget *parent) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::TransparencyDialog),
    mTab(pTab)
{
    ui->setupUi(this);

    mOldSettings = mTab->GetScene().GetTransparencySettings();

    ui->sliderCamera->setValue(mTab->GetScene().GetTransparencySettings().CameraTransparency());
    ui->sliderMapCollision->setValue(mTab->GetScene().GetTransparencySettings().CollisionTransparency());
    ui->sliderMapObjects->setValue(mTab->GetScene().GetTransparencySettings().MapObjectTransparency());
}

TransparencyDialog::~TransparencyDialog()
{
    delete ui;
}

void TransparencyDialog::on_buttonBox_accepted()
{
    mTab->GetScene().SyncTransparencySettings();
}


void TransparencyDialog::on_buttonBox_rejected()
{
    mTab->GetScene().GetTransparencySettings() = mOldSettings;
    mTab->GetScene().SyncTransparencySettings();
}

void TransparencyDialog::on_sliderMapCollision_valueChanged(int value)
{
    mTab->GetScene().GetTransparencySettings().SetCollisionTransparency(value);
    mTab->GetScene().SyncTransparencySettings();
}


void TransparencyDialog::on_sliderCamera_valueChanged(int value)
{
    mTab->GetScene().GetTransparencySettings().SetCameraTransparency(value);
    mTab->GetScene().SyncTransparencySettings();
}


void TransparencyDialog::on_sliderMapObjects_valueChanged(int value)
{
    mTab->GetScene().GetTransparencySettings().SetMapObjectTransparency(value);
    mTab->GetScene().SyncTransparencySettings();
}
