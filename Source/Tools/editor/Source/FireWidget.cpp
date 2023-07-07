#include "FireWidget.hpp"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

FireWidget::FireWidget(QWidget *aParent)
    : QWidget( aParent )
{
    iFireColour_A = 200;
    iFireColourB = 50;
    iFireA = 200;
    iFireB = 50;
    dword_40D39C = 10;
    dword_403067 = 10;

    iFireHeight = 0x9F;    // Height? 159
    iFireColourIndex = 0;


    fp_i = 0;

    memset( g_generated_fire_colours, 0, sizeof(g_generated_fire_colours) );
    memset( mImageBuffer, 0, sizeof(mImageBuffer) );

    iRandSeed = 1;

    // fire_proc() renders into here
    iBuffer = QImage( kWidth, kHeight, QImage::Format_RGB32 );

    init_fire();

    // Redraw/update every 30msec
    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(repaint()) );
    timer->start( 25 );

    this->setMinimumSize( kWidth, kHeight );
    this->setMaximumSize( kWidth, kHeight );
}

void FireWidget::paintEvent( QPaintEvent* aEvent )
{
    // Create a painter for drawing.
    QPainter painter( this );

    // Render fire
    fire_proc( aEvent, painter );
}

void FireWidget::mousePressEvent( QMouseEvent* aEvent )
{
    Q_UNUSED( aEvent );

    // Change the colour when clicked
    change_fire_colour();
}

void FireWidget::change_fire_colour()
{
    int newFireColourIndex; // eax@1
    unsigned char bFireColourLessThan5; // cf@1
    unsigned char bFireColourIndexOutOfRange; // zf@1
    unsigned int result; // eax@4
    signed int fireACopy; // ecx@4
    signed int fireBCopy; // esi@4

    newFireColourIndex = iFireColourIndex + 1;
    bFireColourLessThan5 = (iFireColourIndex + 1) < 5;
    bFireColourIndexOutOfRange = iFireColourIndex++ == 4;
    if ( !(bFireColourLessThan5 | bFireColourIndexOutOfRange) )
    {
        newFireColourIndex = 0;
        iFireColourIndex = 0;
    }

    switch (newFireColourIndex)
    {
    case 1:
        fireACopy = 25;
        fireBCopy = 100;
        iFireColour_A = 25;
        iFireColourB = 100;
        result = 25;
        break;
    case 2: // green
        fireACopy = 10;
        fireBCopy = 50;
        iFireColour_A = 10;
        iFireColourB = 50;
        result = 200;
        break;
    case 3:
        fireACopy = 150;
        fireBCopy = 50;
        iFireColour_A = 150;
        iFireColourB = 50;
        result = 150;
        break;
    case 4:
        fireACopy = 150;
        result = 50;
        fireBCopy = 150;
        iFireColour_A = 150;
        iFireColourB = 150;
        break;
    case 5: // black and white
        fireACopy = 64;
        fireBCopy = 64;
        iFireColour_A = 64;
        iFireColourB = 64;
        result = 64;
        break;
    default:
        fireACopy = 200;
        fireBCopy = 50;
        iFireColour_A = 200;
        iFireColourB = 50;
        result = 10;
        break;
    }

    iFireA = fireBCopy;
    iFireB = fireACopy;
    dword_40D39C = result;
    dword_403067 = result;  

    init_fire_tab();
}

void FireWidget::init_fire_tab()
{
    unsigned int v12; // eax@12
    unsigned int v13; // edx@18
    int v14; // ebx@20
    int colour; // ebx@20
    int v16; // eax@20
    int v17; // edx@20
    char lastScanLine; // zf@20

    unsigned int result = dword_403067;
    unsigned int v7 = 0;
    unsigned int v9 = 0;
    unsigned int v10 = 0;

    unsigned int dword_40D3E0;
    unsigned int v8 = iFireA;

    // CODE DUP
    int *fireTabPtr = (int*)&g_generated_fire_colours;
    fireHeightPlusOne = iFireHeight + 1;
    do
    {
        if ( (result & 0xFFFFFC00) >= 0x3FC00 )
        {
            v12 |= 0xFF;
            //LOBYTE(v12) = -1;
        }
        else
        {
            v12 = result >> 10;
        }

        if ( (v8 & 0xFFFFFC00) >= 0x3FC00 )
        {
            dword_40D3E0 = 0xFF;
        }
        else
        {
            dword_40D3E0 = v8 >> 10;
        }

        if ( (iFireB & 0xFFFFFC00u) >= 0x3FC00 )
        {
            v13 |= 0xFF;
            //LOBYTE(v13) = -1;
        }
        else
        {
            v13 = (unsigned int)iFireB >> 10;
        }

        // HIWORD(v14) = 0;
        v14 = 0;
        //	v14 = v14 & 0x00FF;

        //BYTE1(v14) = v13;

        unsigned short int t = (unsigned short int)(v13 << 8);
        v14 |= (t& 0xFF00);

        //LOBYTE(v14) = dword_40D3E0;
        v14 |= (unsigned char)dword_40D3E0;

        colour = (unsigned char)v12 | (v14 << 8);

        v16 = dword_40D39C;
        iFireB += v10;
        v17 = iFireColour_A;
        *fireTabPtr = colour; // Store the colour
        v10 += v17;
        v8 = v9 + iFireA;
        v9 += iFireColourB;
        result = v7 + v16;
        v7 += dword_403067;
        ++fireTabPtr;
        lastScanLine = fireHeightPlusOne == 1;
        iFireA = v8;
        dword_40D39C = result;
        --fireHeightPlusOne;
    }
    while ( !lastScanLine );
}

void FireWidget::init_fire()
{
    iFireA = iFireColourB;
    iFireB = iFireColour_A;
    dword_40D39C = dword_403067;

    init_fire_tab();
}

void FireWidget::fire_proc( QPaintEvent* /*aEvent*/, QPainter& aPainter )
{
    //while ( 1 )
    {
        for ( ; fp_i & 7; ++fp_i )
        {
            int y = 0;
            int x = 0;
            //rgb_src_dword = &g_bmpBitsPtr[0];
            unsigned char *v2 = &mImageBuffer[0];                       // start of data

            // Fixed loop - will do one scanline?
            do
            {
                x = 0;
                //bmpPosIndex = 0;
                do
                {
                    x++;

                    unsigned char fire_colour_index = (v2[kWidth]
                                         + v2[1]
                                         + *v2
                                         + *(v2 - 1)
                                         + v2[kWidth-1]
                                         + v2[kWidth+1]
                                         + v2[(kWidth*2)-1]
                                         + v2[(kWidth*2)+1]
                                         + 8 * (unsigned int)v2[kWidth*2]) >> 4;

                    (v2)[kWidth] = fire_colour_index;
                    v2++;

                    unsigned int pixel = g_generated_fire_colours[fire_colour_index];

                    int realx = x-1;


                    iBuffer.setPixel( realx, y, pixel );
                }
                while ( x < kWidth );
                y++;
            }
            // 4035CE to 40B6B6 = 0x80E8
            // 33,000 data
            // 330*100=  33 000!
            while ( v2 < &mImageBuffer[kWidth*kHeight] );// ptr to end of data

            unsigned char *v7 = &mImageBuffer[kWidth*kHeight];
            do
            {
                signed int widthCounterA = kWidth;
                do
                {
                    unsigned char value = (*v7
                                  + v7[kWidth]
                                  + v7[1]
                                  + *(v7 - 1)
                                  + v7[kWidth-1]
                                  + v7[kWidth+1]
                                  + v7[(kWidth*2)-1]
                                  + v7[(kWidth*2)+1] // 0x9F!
                                  + 8 * (unsigned int)v7[kWidth*2]) >> 4;

                    v7[kWidth] = value;
                    ++v7;
                    --widthCounterA;
                }
                while ( widthCounterA );
            }
            // 40B6B6 to 40CCA0 = 0x15EA
            // 5610 = ? 5610 / 330 = 17 why 17 lines?
            while ( v7 < &mImageBuffer[(kWidth*kHeight)+(17*kWidth)] );

            //++v15;                                    // not used?
            //const unsigned int wDest = 0x14A;
            //const unsigned int hDest = 0x64;
            aPainter.drawImage( 0, 0, iBuffer );
            //StretchBlt(v16, 0, 0, wDest, hDest, hdc, 0, 0, 330, 100, SRCCOPY);
        }
        ++fp_i;
        signed int widthCounterB = 1;

        // Doing ONE line only? The seed line?
        do
        {
            char randNum = fire_rand();                         // Using dword at 40304F
            if ( randNum & 8 )
            {
                int v12 = randNum & 7;
                int v11 = (randNum & 7) - 1;
                if ( v12 )
                {
                    unsigned char v13 = 0x9F; // g_k0x9F;
                    do
                    {
                        if ( widthCounterB >= kWidth )
                        {
                            break;
                        }
                        // start at 40CDEA
                        // so 0x40CDEA - 0x4035CE = 0x981C
                        // 38 940 = / 300 =
                        // 0x981C = 38940
                        // 38940 - (330*100) = 5940
                        // 5940 - 5610 = 330
                        // backwards one line into the previous data + one more line?
                        // ((kWidth*kHeight)+(17*kWidth)+kWidth) = 38 280 =
                        mImageBuffer[((kWidth*kHeight)+(17*kWidth)+kWidth)+widthCounterB++] = v13;
                        mV14 = v11--;
                    }
                    while ( mV14 );
                }
            }
            else
            {
                mImageBuffer[((kWidth*kHeight)+(17*kWidth)+kWidth)+widthCounterB++] = 0;
            }
            ++widthCounterB;
        }
        while ( widthCounterB < kWidth );// 330 is bitmap width
        // Sleep(35u);
    }
}

// Pretty much rand()
int FireWidget::fire_rand()
{
    iRandSeed = 214013 * iRandSeed + 2531011;
    return (iRandSeed >> 16) & 0x7FFF;
}
