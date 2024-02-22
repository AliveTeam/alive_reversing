#include "ColourPickerProperty.hpp"
#include <QColorDialog>

static void UpdateLabelColour(QLabel* l, const RGB16& rgb)
{
    l->setStyleSheet("QLabel { background-color : rgba("+ QString("%1,%2,%3,255").arg(rgb.r).arg(rgb.g).arg(rgb.b) +"); }");
}

static QLabel* MakeLabel(QWidget* pParent, const RGB16& rgb)
{
    QLabel* l = new QLabel(pParent);
    l->setAutoFillBackground(true);
    UpdateLabelColour(l, rgb);
    return l;
}

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
    mLabel = MakeLabel(pParent, mProperty);
    mPrevValue = mProperty;
}

QWidget* ColourPickerProperty::GetEditorWidget(PropertyTreeWidget* pParent)
{
    auto colorDialog = new QColorDialog(pParent);
    connect(colorDialog, &QColorDialog::colorSelected, this, [this, pParent](const QColor& color)
    {
        mUndoStack.push(new ChangeColourPickerPropertyCommand(pParent, mProperty, text(0), mPrevValue, QColorToRGB16(color)));
        mPrevValue = mProperty;
    });
    colorDialog->open(); // This opens a blocking dialog that is auto deleted when the user closes it

    // Although we just created a blocking dialog we don't want to set it as the tree widget
    return nullptr;
}

QWidget* ColourPickerProperty::GetPersistentEditorWidget(PropertyTreeWidget* /*pParent*/)
{
    // The persistent widget that shows the active colour
    return mLabel;
}

void ColourPickerProperty::Refresh()
{
    UpdateLabelColour(mLabel, mProperty);
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
