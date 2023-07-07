#include "BigSpinBox.hpp"
#include <QLineEdit>
#include <assert.h>
#include <limits>

BigSpinBox::BigSpinBox( QWidget* aParent )
    : QAbstractSpinBox( aParent ),
      mMinRange( std::numeric_limits<qint64>::min() ),
      mMaxRange( std::numeric_limits<qint64>::max() )
{
    updateLineEdit();

    connect( this, SIGNAL(editingFinished()), this, SLOT(OnEditComplete()));
}

BigSpinBox::~BigSpinBox()
{

}

void BigSpinBox::setMax( qint64 aMax )
{
    mMaxRange = aMax;
}

void BigSpinBox::setMin( qint64 aMin )
{
    mMinRange = aMin;
}

void BigSpinBox::setRange( qint64 aMin, qint64 aMax )
{
    assert( aMin < aMax );
    setMin( aMin );
    setMax( aMax );
}

qint64 BigSpinBox::value() const
{
    return mValue;
}

void BigSpinBox::setValue( qint64 aValue, bool aEmitChangedSignal )
{
    if ( aValue > mMaxRange )
    {
        aValue = mMaxRange;
    }

    if ( aValue < mMinRange )
    {
        aValue = mMinRange;
    }

    if ( aValue != mValue )
    {
        mValue = aValue;
        mOldValue = mValue;
        updateLineEdit();
        if ( aEmitChangedSignal )
        {
            emit valueChanged( mValue, false );
        }
    }
}

void BigSpinBox::OnEditComplete()
{
    if ( mOldValue != mValue )
    {
        mOldValue = mValue;
        emit valueChanged( mValue, true );
    }
}

void BigSpinBox::stepBy( int aSteps )
{
    qint64 temp = mValue;
    temp += aSteps;
    if ( temp > mMaxRange )
    {
        temp = mMaxRange;
    }

    if ( temp < mMinRange )
    {
        temp = mMinRange;
    }

    if ( mValue != temp )
    {
        mValue = temp;
        mOldValue = mValue;
        updateLineEdit();
        emit valueChanged( mValue, false );
    }
}

void BigSpinBox::fixup( QString& aInput ) const
{

}

QValidator::State BigSpinBox::validate( QString& aInput, int& aPos ) const
{
    bool ok = false;
    qint64 value = aInput.toLongLong( &ok );
    if (!ok && aInput.isEmpty() || (aInput == "-" && mMinRange < 0))
    {
        // Special case to allow deleting all of the spinbox input, when deleted we use a value of the min range
        // or when entering just "-" and our range allows negative numbers
        value = mMinRange;
        ok = true;
    }

    if ( ok )
    {
        if ( value > mMaxRange )
        {
            return QValidator::Invalid;
        }

        if ( value < mMinRange )
        {
            return QValidator::Invalid;
        }

        if ( mValue != value )
        {
            mOldValue = mValue;
            mValue = value;
        }
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}

QAbstractSpinBox::StepEnabled BigSpinBox::stepEnabled() const
{
    // StepNone
    return StepUpEnabled | StepDownEnabled;
}

void BigSpinBox::updateLineEdit()
{
    QLineEdit * const edit = lineEdit();
    if ( edit )
    {
        const QString str = QString::number( mValue );
        edit->setText( str );
    }
}
