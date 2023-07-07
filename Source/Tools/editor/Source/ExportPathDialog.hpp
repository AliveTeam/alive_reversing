#ifndef EXPORTPATHDIALOG_HPP
#define EXPORTPATHDIALOG_HPP

#include <QDialog>

namespace Ui {
    class ExportPathDialog;
}

namespace ReliveAPI {
    class Context;
}

class ExportPathDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportPathDialog(QWidget *parent, bool exportAndPlay);
    ~ExportPathDialog();

    void ExportAndPlay();
    void setJsonPath(QString path);
    void setLvlName(QString path);
    void setExtraLvlsPath(QString path);
    void setRelivePath(QString path);
    QString getLvlName() const;
    QString getRelivePath() const;
    QString getJsonPath() const;
    QString getExtraLvlsPath() const;
private slots:
    void on_btnSelectJson_clicked();

    void on_btnSelectLvlFile_clicked();

    void on_btnSelectRelive_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_btnSelectExtraLvlsDir_clicked();

private:
    bool ExportToLvl(ReliveAPI::Context& context);
    
    Ui::ExportPathDialog *ui;
    bool mExportAndPlay = false;
};

#endif // EXPORTPATHDIALOG_HPP
