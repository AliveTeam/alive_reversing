#pragma once

#include <QList>
#include <QGraphicsItem>
#include "EditorGraphicsScene.hpp"
#include "EditorTab.hpp"

class SelectionSaver final
{
public:
    explicit SelectionSaver(EditorTab* pTab)
        : mTab(pTab)
    {
        mOldSelection = mTab->GetScene().selectedItems();
    }

    void undo()
    {
        // Select whatever was selected previously
        for (auto& item : mOldSelection)
        {
            item->setSelected(true);
        }

        // Redraw
        mTab->GetScene().update();

        // Ensure the property editor is displaying the correct item
        mTab->SyncPropertyEditor();
    }

    void redo()
    {
        mTab->GetScene().update();
        mTab->SyncPropertyEditor();
    }

private:
    EditorTab* mTab = nullptr;
    QList<QGraphicsItem*> mOldSelection;
};
