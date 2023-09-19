#include "ExportPathDialog.hpp"
#include "ui_ExportPathDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QUuid>
#include "relive_api.hpp"
#include <QProcess>
#include "ShowContext.hpp"

ExportPathDialog::ExportPathDialog(QWidget *parent, bool exportAndPlay) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::ExportPathDialog),
    mExportAndPlay(exportAndPlay)
{
    ui->setupUi(this);

    // If the dialog is being called for export & play then do it without any user interaction if the json/lvl/exe paths seem valid
    if (mExportAndPlay)
    {
        if (QFileInfo(getJsonPath()).exists() && QFileInfo(getLvlName()).exists() && QFileInfo(getRelivePath()).exists())
        {
            ExportAndPlay();
            close();
        }
    }
}

ExportPathDialog::~ExportPathDialog()
{
    delete ui;
}

void ExportPathDialog::on_btnSelectJson_clicked()
{
    QString jsonFileName = QFileDialog::getOpenFileName(this, tr("Save path json"), "", tr("Json Files (*.json);;All Files (*)"));
    if (!jsonFileName.isEmpty())
    {
        setJsonPath(jsonFileName);
    }
}

void ExportPathDialog::on_btnSelectLvlFile_clicked()
{
    QString lvlFileName = QFileDialog::getOpenFileName(this, tr("Open level"), "", tr("Level Files (*.lvl);;All Files (*)"));
    if (!lvlFileName.isEmpty())
    {
        setLvlName(lvlFileName);
    }
}

void ExportPathDialog::on_btnSelectRelive_clicked()
{
    QString reliveExe = QFileDialog::getOpenFileName(this, tr("Select R.E.L.I.V.E executable"), "");
    if (!reliveExe.isEmpty())
    {
        setRelivePath(reliveExe);
    }
}

void ExportPathDialog::ExportAndPlay()
{
    ReliveAPI::Context context;
    if (ExportToLvl(context))
    {
        if (!context.Ok())
        {
            ShowContext(context);
        }

        if (!ui->txtRelivePath->text().isEmpty())
        {
            QString reliveExe = ui->txtRelivePath->text();
            QFileInfo info(reliveExe);

            if (!info.isExecutable())
            {
                QMessageBox::critical(this, "Error", "Failed to open R.E.L.I.V.E, the selected file is not an executable");
                return;
            }
            QProcess* process = new QProcess(this);
            process->setWorkingDirectory(info.dir().path());
            process->setProgram(reliveExe);
            process->setArguments(QStringList{});
            process->startDetached();
            delete process;
        }
    }
}

bool ExportPathDialog::ExportToLvl(ReliveAPI::Context& context)
{
    auto jsonPath = ui->txtJsonPath->text();
    auto lvlPath = ui->txtLvlFilePath->text();
    auto partialTemporaryFilePath = qApp->applicationName().replace(" ", "");

    QFileInfoList lvls;
    if (!ui->txtExtraLvlsLocation->text().isEmpty())
    {
        QDir directory(ui->txtExtraLvlsLocation->text());
        lvls = directory.entryInfoList(QStringList() << "*.lvl" << "*.LVL", QDir::Files);
    }

    std::set<std::string> stdLvls;
    for (auto i = 0; i < lvls.size(); i++)
    {
        stdLvls.insert(lvls[i].absoluteFilePath().toStdString());
    }

    /*
    return exportJsonToLvl(jsonPath, lvlPath, partialTemporaryFilePath, [&](const QString text)
        {
            QMessageBox::critical(this, "Error", text);
        }, stdLvls, context);
        */
    QMessageBox::critical(this, "Error", "Exporting to lvls is obsolete now, this will be removed soon");
    return false;
}

void ExportPathDialog::on_buttonBox_accepted()
{
    ReliveAPI::Context context;
    if (ExportToLvl(context))
    {
        if (!context.Ok())
        {
            ShowContext(context);
        }
        else
        {
            if (mExportAndPlay)
            {
                ExportAndPlay();
            }
        }
    }
}

void ExportPathDialog::on_buttonBox_rejected()
{
    ui->txtLvlFilePath->clear(); // don't persist the bad path
}

void ExportPathDialog::setJsonPath(QString path)
{
    ui->txtJsonPath->setText(path);
}

void ExportPathDialog::setLvlName(QString path)
{
    ui->txtLvlFilePath->setText(path);
    if (ui->txtExtraLvlsLocation->text().isEmpty())
    {
        ui->txtExtraLvlsLocation->setText(QFileInfo(path).absoluteDir().path());
    }
}

void ExportPathDialog::setExtraLvlsPath(QString path)
{
    ui->txtExtraLvlsLocation->setText(path);
}

void ExportPathDialog::setRelivePath(QString path)
{
    ui->txtRelivePath->setText(path);
}

QString ExportPathDialog::getLvlName() const
{
    return ui->txtLvlFilePath->text();
}

QString ExportPathDialog::getRelivePath() const
{
    return ui->txtRelivePath->text();
}

QString ExportPathDialog::getJsonPath() const
{
    return ui->txtJsonPath->text();
}

QString ExportPathDialog::getExtraLvlsPath() const
{
    return ui->txtExtraLvlsLocation->text();
}

void ExportPathDialog::on_btnSelectExtraLvlsDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select LVLs source dir"),
        "",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        ui->txtExtraLvlsLocation->setText(dir);
    }
}

