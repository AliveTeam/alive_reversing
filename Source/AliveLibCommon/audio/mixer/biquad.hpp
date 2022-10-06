/* Simple implementation of Biquad filters -- Tom St Denis
 *
 * Based on the work

Cookbook formulae for audio EQ biquad filter coefficients
---------------------------------------------------------
by Robert Bristow-Johnson, pbjrbj@viconet.com  a.k.a. robert@audioheads.com

 * Available on the web at

http://www.smartelectronix.com/musicdsp/text/filters005.txt

 * Enjoy.
 *
 * This work is hereby placed in the public domain for all purposes, whether
 * commercial, free [as in speech] or educational, etc.  Use the code and please
 * give me credit if you wish.
 *
 * Tom St Denis -- http://tomstdenis.home.dhs.org
*/

#pragma once

namespace psx {

/* this would be biquad.h */
#include <math.h>
#include <stdlib.h>

#ifndef M_LN2
    #define M_LN2 0.69314718055994530942
#endif

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/* whatever sample type you want */
typedef float smp_type;

/* this holds the data required to update samples thru a filter */
typedef struct
{
    smp_type a0, a1, a2, a3, a4;
    smp_type x1, x2, y1, y2;
} biquad;

/* filter types */
enum
{
    LPF,   /* low pass filter */
    HPF,   /* High pass filter */
    BPF,   /* band pass filter */
    NOTCH, /* Notch Filter */
    PEQ,   /* Peaking band EQ filter */
    LSH,   /* Low shelf filter */
    HSH    /* High shelf filter */
};

smp_type BiQuad(smp_type sample, biquad* b);
biquad* BiQuad_new(int type, smp_type dbGain, /* gain of filter */
                   smp_type freq,             /* center frequency */
                   smp_type srate,            /* sampling rate */
                   smp_type bandwidth);       /* bandwidth in octaves */

} // namespace psx