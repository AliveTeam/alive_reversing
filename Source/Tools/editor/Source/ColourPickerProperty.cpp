#include "ColourPickerProperty.hpp"
#include <QColorDialog>

static RGB16 QColorToRGB16(const QColor& qcolor)
{
    RGB16 rgb;
    rgb.r = qcolor.red();
    rgb.g = qcolor.green();
    rgb.b = qcolor.blue();

    return rgb;
}

static QString RGB16ToString(const RGB16& rgb)
{
    return QString("%1,%2,%3").arg(rgb.r).arg(rgb.g).arg(rgb.b);
}

ColourPickerProperty::ColourPickerProperty(PropertyTreeWidget* pParent, RGB16& pProperty, const char* pPropertyName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem)
    : PropertyTreeItemBase(pParent)
    , mProperty(pProperty)
    , mPropertyName(pPropertyName)
    , mUndoStack(undoStack)
    , mGraphicsItem(pGraphicsItem)
{
    setText(0, mPropertyName);
    Refresh();

    mPrevValue = mProperty;
}

QWidget* ColourPickerProperty::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    setText(0, mPropertyName);
    auto colorDialog = new QColorDialog(pParent);
    colorDialog->open();

    connect(colorDialog, &QColorDialog::colorSelected, this, [this, colorDialog, pParent](const QColor& color)
    {
        mUndoStack.push(new ChangeColourPickerPropertyCommand(pParent, mProperty, text(0), mPrevValue, QColorToRGB16(color)));
        mPrevValue = mProperty;
        pParent->setItemWidget(this, 1, nullptr);
    });

    return colorDialog;
}

void ColourPickerProperty::Refresh()
{
    setText(1, RGB16ToString(mProperty));
}

ChangeColourPickerPropertyCommand::ChangeColourPickerPropertyCommand(PropertyTreeWidget* pTreeWidget, RGB16& pProperty, QString propertyName, RGB16 oldValue, RGB16 newValue)
    : mTreeWidget(pTreeWidget)
    , mProperty(pProperty)
    , mOldValue(oldValue)
    , mNewValue(newValue)
{
    setText(QString("Change property %1 from %2 to %3").arg(propertyName).arg(RGB16ToString(oldValue)).arg(RGB16ToString(newValue)));
}

void ChangeColourPickerPropertyCommand::undo()
{
    mProperty = mOldValue;
    mTreeWidget->FindObjectPropertyByKey(&mProperty)->Refresh();
}

void ChangeColourPickerPropertyCommand::redo()
{
    mProperty = mNewValue;
    mTreeWidget->FindObjectPropertyByKey(&mProperty)->Refresh();
}
