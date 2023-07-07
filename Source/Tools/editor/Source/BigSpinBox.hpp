#pragma once

#include <QAbstractSpinBox>

class BigSpinBox final : public QAbstractSpinBox
{
    Q_OBJECT
public:
    BigSpinBox( QWidget* aParent = nullptr );
    virtual ~BigSpinBox();
    void setMax( qint64 aMax );
    void setMin( qint64 aMin );
    void setRange( qint64 aMin, qint64 aMax );
    qint64 value() const;
    void setValue( qint64 aValue, bool aEmitChangedSignal = true );
signals:
    void valueChanged( qint64 aValue, bool closeEditor ) const;
private slots:
    void OnEditComplete();
protected:
    virtual void stepBy( int aSteps ) override;
    virtual void fixup( QString& aInput ) const override;
    virtual QValidator::State validate( QString& aInput, int& aPos ) const override;
    virtual StepEnabled stepEnabled() const override;
private:
    void updateLineEdit();
private:
    mutable qint64 mValue = 0;
    mutable qint64 mOldValue = 0;
    qint64 mMinRange = 0;
    qint64 mMaxRange = 0;
};
