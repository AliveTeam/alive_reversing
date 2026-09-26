#pragma once

#include "PropertyTreeItemBase.hpp"

class ReadOnlyStringProperty : public PropertyTreeItemBase
{
public:
    ReadOnlyStringProperty(QTreeWidgetItem* pParent, QString propertyName, QString propertyValue);

    virtual QWidget* GetEditorWidget(PropertyTreeWidget* ) override
    {
        return nullptr;
    }

    void Refresh() override
    {

    }

    const void* GetPropertyLookUpKey() const override
    {
        return nullptr;
    }
};
