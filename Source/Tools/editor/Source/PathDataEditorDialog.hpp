#ifndef PATHDATAEDITORDIALOG_HPP
#define PATHDATAEDITORDIALOG_HPP

#include <QDialog>

namespace Ui {
class PathDataEditorDialog;
}

class EditorTab;

class PathDataEditorDialog : public QDialog
{
    Q_OBJECT

public:
    PathDataEditorDialog(QWidget *parent, EditorTab* pTab);
    ~PathDataEditorDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PathDataEditorDialog *ui;
    EditorTab* mTab = nullptr;
};

#endif // PATHDATAEDITORDIALOG_HPP
