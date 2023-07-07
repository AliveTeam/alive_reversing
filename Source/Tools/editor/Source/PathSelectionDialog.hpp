#ifndef PATHSELECTIONDIALOG_HPP
#define PATHSELECTIONDIALOG_HPP

#include <QDialog>
#include <optional>

namespace ReliveAPI {
    struct EnumeratePathsResult;
}

namespace Ui {
class PathSelectionDialog;
}

class PathSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PathSelectionDialog(QWidget *parent, ReliveAPI::EnumeratePathsResult& paths);
    ~PathSelectionDialog();

    std::optional<int> SelectedPath() const
    {
        return mSelectedPath;
    }

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PathSelectionDialog *ui;
    std::optional<int> mSelectedPath;
    ReliveAPI::EnumeratePathsResult& mPaths;
};

#endif // PATHSELECTIONDIALOG_HPP
