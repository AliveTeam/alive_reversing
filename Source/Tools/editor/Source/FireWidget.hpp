#pragma once

#include <QWidget>

const static int kWidth = 480;
const static int kHeight = 100;

class FireWidget final : public QWidget
{
public:
    FireWidget( QWidget* aParent = nullptr );
protected:
    void paintEvent( QPaintEvent* aEvent ) override;
    void mousePressEvent( QMouseEvent* aEvent ) override;
private:
    void change_fire_colour();
    void init_fire_tab();
    void init_fire();
    void fire_proc( QPaintEvent* aEvent, QPainter& aPainter );
    int fire_rand();
private:
    unsigned int iFireColourIndex;
    unsigned int iFireColour_A;
    unsigned int iFireColourB;
    unsigned int iFireB;
    unsigned int dword_403067;
    unsigned int iFireA;
    unsigned int dword_40D39C;

    unsigned int fireHeightPlusOne;
    unsigned int iFireHeight;

    unsigned int g_generated_fire_colours[0x9F+1];

    unsigned char mImageBuffer[(kWidth*kHeight)+(17*kWidth)+kWidth+kWidth];
    unsigned int iRandSeed;
    int fp_i;
    int mV14;

    QImage iBuffer;
};
