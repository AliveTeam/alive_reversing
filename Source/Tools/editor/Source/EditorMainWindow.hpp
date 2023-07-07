#pragma once

#include <QMainWindow>
#include <memory>
#include <QSettings>
#include "EditorTab.hpp"
#include "ClipBoard.hpp"
#include "SnapSettings.hpp"

namespace Ui
{
    class EditorMainWindow;
}

class EditorMainWindow final : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit EditorMainWindow( QWidget* aParent = 0 );
    ~EditorMainWindow();
private slots:
    void on_actionExport_and_play_triggered();

    void on_actionDark_Fusion_theme_triggered();

    void on_actionDark_theme_triggered();

    void on_action_about_qt_triggered();

    void on_action_about_triggered();

    void onCloseTab(int index);

    void on_action_open_path_triggered();

    void on_action_zoom_reset_triggered();

    void on_action_zoom_in_triggered();

    void on_action_zoom_out_triggered();

    void on_action_undo_triggered();

    void on_action_redo_triggered();

    void on_action_save_path_triggered();

    void on_actionSave_all_triggered();

    void on_actionExport_to_lvl_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_actionEdit_HintFly_messages_triggered();

    void on_actionEdit_LCDScreen_messages_triggered();

    void on_actionEdit_path_data_triggered();

    void on_actionEdit_map_size_triggered();

    void on_actionNew_path_triggered();

    void on_actionSave_As_triggered();

    void on_actionAdd_object_triggered();

    void on_actionAdd_collision_triggered();

    void on_actionConnect_collisions_triggered();

    void on_action_close_path_triggered();

    void on_actionItem_transparency_triggered();

    void on_action_toggle_show_grid_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_action_snap_collision_items_on_x_toggled(bool on);

    void on_action_snap_map_objects_x_toggled(bool on);

    void on_action_snap_collision_objects_on_y_toggled(bool on);

    void on_action_snap_map_objects_y_toggled(bool on);

private:
    void readSettings();
    void setMenuActionsEnabled(bool enable);
    bool onOpenPath(QString fileName, bool createNewPath);
    void UpdateWindowTitle();
    void DisconnectTabSignals();
    void closeEvent(QCloseEvent* pEvent) override;
private:
    Ui::EditorMainWindow* m_ui;
    QSettings m_Settings;
    ClipBoard mClipBoard;
    // Global to all tabs
    SnapSettings mSnapSettings;
    QString mUnthemedStyle;
};
