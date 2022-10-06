#pragma once

#include <iostream>
#include "SDL_Types.h"

// All of the ADSR calculations herein (except where inaccurate) are derived from Neill Corlett's work in
// reverse-engineering the Playstation 1/2 SPU unit.

//**************************************************************************************************
// Type Redefinitions


class DLSArt;

static unsigned long RateTable[160];
static bool bRateTableInitialized = 0;

// This next function converts seconds to full attenuation in a linear amplitude decay scale
// and approximates the time to full attenuation in a linear DB decay scale.
double LinAmpDecayTimeToLinDBDecayTime(double secondsToFullAtten, int linearVolumeRange);

typedef struct _REAL_ADSR
{
    double attack_time;
    double decay_time;
    double sustain_level;
    double sustain_time;
    double release_time;
} REAL_ADSR;

//VAG format -----------------------------------
typedef struct _VAGHdr //File Header
{
    unsigned int id;  //ID - "VAGp"
    unsigned int ver; //Version - 0x20
    unsigned int __r1;
    unsigned int len;  //Length of data
    unsigned int rate; //Sample rate
    unsigned int __r2[3];
    char title[32];
} VAGHdr;


typedef struct _VAGBlk //Sample Block
{
    struct
    {
        unsigned char range : 4;
        unsigned char filter : 4;
    } other;

    struct
    {
        unsigned char end : 1;     //End block
        unsigned char looping : 1; //VAG loops
        unsigned char loop : 1;    //Loop start point
    } flag;

    char brr[14]; //Compressed samples
} VAGBlk;


//InitADSR is shamelessly ripped from P.E.Op.S. cause I'm lazy
static void InitADSR(void);

inline int RoundToZero(int val)
{
    if (val < 0)
        val = 0;
    return val;
}

void PSXConvADSR(REAL_ADSR* realADSR, unsigned short ADSR1, unsigned short ADSR2, bool bPS2);

void PSXConvADSR(REAL_ADSR* realADSR,
                 uint8_t Am, uint8_t Ar, uint8_t Dr, uint8_t Sl,
                 uint8_t Sm, uint8_t Sd, uint8_t Sr, uint8_t Rm, uint8_t Rr, bool bPS2);