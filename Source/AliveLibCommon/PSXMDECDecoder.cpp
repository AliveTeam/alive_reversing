/*
 * PSX MDEC Movie Decoder
 *
 * Copyright (C) (2006-2007) by G. <gennadiy.brich@gmail.com>
 *
 * original code based on libbs from psxdev-libs-2.0.0 by:
 *    Daniel Balster <dbalster@psxdev.de>
 *    Sergio Moreira <sergio@x-plorer.co.uk>
 *    Andrew Kieschnick <andrewk@cerc.utexas.edu>
 *    Kazuki Sakamoto <bsd-ps@geocities.co.jp>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <memory.h>

#include "PSXMDECDecoder.h"
#include "Types.hpp"

// This tables based on MPEG2DEC by MPEG Software Simulation Group
#define CODE1(a,b,c) (((a)<<10)|((b)&0x3ff)|((c)<<16))
#define CODE(a,b,c) CODE1(a,b,c+1),CODE1(a,-b,c+1)
#define CODE0(a,b,c) CODE1(a,b,c),CODE1(a,b,c)
#define CODE2(a,b,c) CODE1(a,b,c+1),CODE1(a,b,c+1)


// Table B-14, DCT coefficients table zero,
// codes 0100 ... 1xxx (used for all other coefficients)
const uint32_t PSXMDECDecoder::VLC_TABLE_NEXT[12 * 2] =
{
    CODE(0, 2, 4), CODE(2, 1, 4), CODE2(1, 1, 3), CODE2(1, -1, 3),
    CODE0(63, 512, 2), CODE0(63, 512, 2), CODE0(63, 512, 2), CODE0(63, 512, 2), // EOB
    CODE2(0, 1, 2), CODE2(0, 1, 2), CODE2(0, -1, 2), CODE2(0, -1, 2)
};


// Table B-14, DCT coefficients table zero,
// codes 000001xx ... 00111xxx
const uint32_t PSXMDECDecoder::VLC_TABLE_0[60 * 2] =
{
    CODE0(63, 0, 6), CODE0(63, 0, 6), CODE0(63, 0, 6), CODE0(63, 0, 6), // ESCAPE
    CODE2(2, 2, 7), CODE2(2, -2, 7), CODE2(9, 1, 7), CODE2(9, -1, 7),
    CODE2(0, 4, 7), CODE2(0, -4, 7), CODE2(8, 1, 7), CODE2(8, -1, 7),
    CODE2(7, 1, 6), CODE2(7, 1, 6), CODE2(7, -1, 6), CODE2(7, -1, 6),
    CODE2(6, 1, 6), CODE2(6, 1, 6), CODE2(6, -1, 6), CODE2(6, -1, 6),
    CODE2(1, 2, 6), CODE2(1, 2, 6), CODE2(1, -2, 6), CODE2(1, -2, 6),
    CODE2(5, 1, 6), CODE2(5, 1, 6), CODE2(5, -1, 6), CODE2(5, -1, 6),
    CODE(13, 1, 8), CODE(0, 6, 8), CODE(12, 1, 8), CODE(11, 1, 8),
    CODE(3, 2, 8), CODE(1, 3, 8), CODE(0, 5, 8), CODE(10, 1, 8),
    CODE2(0, 3, 5), CODE2(0, 3, 5), CODE2(0, 3, 5), CODE2(0, 3, 5),
    CODE2(0, -3, 5), CODE2(0, -3, 5), CODE2(0, -3, 5), CODE2(0, -3, 5),
    CODE2(4, 1, 5), CODE2(4, 1, 5), CODE2(4, 1, 5), CODE2(4, 1, 5),
    CODE2(4, -1, 5), CODE2(4, -1, 5), CODE2(4, -1, 5), CODE2(4, -1, 5),
    CODE2(3, 1, 5), CODE2(3, 1, 5), CODE2(3, 1, 5), CODE2(3, 1, 5),
    CODE2(3, -1, 5), CODE2(3, -1, 5), CODE2(3, -1, 5), CODE2(3, -1, 5)
};


// Table B-14, DCT coefficients table zero,
// codes 0000001000 ... 0000001111
const uint32_t PSXMDECDecoder::VLC_TABLE_1[8 * 2] =
{
    CODE(16, 1, 10), CODE(5, 2, 10), CODE(0, 7, 10), CODE(2, 3, 10),
    CODE(1, 4, 10), CODE(15, 1, 10), CODE(14, 1, 10), CODE(4, 2, 10)
};


// Table B-14/15, DCT coefficients table zero / one,
// codes 000000010000 ... 000000011111
const uint32_t PSXMDECDecoder::VLC_TABLE_2[16 * 2] =
{
    CODE(0, 11, 12), CODE(8, 2, 12), CODE(4, 3, 12), CODE(0, 10, 12),
    CODE(2, 4, 12), CODE(7, 2, 12), CODE(21, 1, 12), CODE(20, 1, 12),
    CODE(0, 9, 12), CODE(19, 1, 12), CODE(18, 1, 12), CODE(1, 5, 12),
    CODE(3, 3, 12), CODE(0, 8, 12), CODE(6, 2, 12), CODE(17, 1, 12)
};


// Table B-14/15, DCT coefficients table zero / one,
// codes 0000000010000 ... 0000000011111
const uint32_t PSXMDECDecoder::VLC_TABLE_3[16 * 2] =
{
    CODE(10, 2, 13), CODE(9, 2, 13), CODE(5, 3, 13), CODE(3, 4, 13),
    CODE(2, 5, 13), CODE(1, 7, 13), CODE(1, 6, 13), CODE(0, 15, 13),
    CODE(0, 14, 13), CODE(0, 13, 13), CODE(0, 12, 13), CODE(26, 1, 13),
    CODE(25, 1, 13), CODE(24, 1, 13), CODE(23, 1, 13), CODE(22, 1, 13)
};


// Table B-14/15, DCT coefficients table zero / one,
// codes 00000000010000 ... 00000000011111
const uint32_t PSXMDECDecoder::VLC_TABLE_4[16 * 2] =
{
    CODE(0, 31, 14), CODE(0, 30, 14), CODE(0, 29, 14), CODE(0, 28, 14),
    CODE(0, 27, 14), CODE(0, 26, 14), CODE(0, 25, 14), CODE(0, 24, 14),
    CODE(0, 23, 14), CODE(0, 22, 14), CODE(0, 21, 14), CODE(0, 20, 14),
    CODE(0, 19, 14), CODE(0, 18, 14), CODE(0, 17, 14), CODE(0, 16, 14)
};


// Table B-14/15, DCT coefficients table zero / one,
// codes 000000000010000 ... 000000000011111
const uint32_t PSXMDECDecoder::VLC_TABLE_5[16 * 2] =
{
    CODE(0, 40, 15), CODE(0, 39, 15), CODE(0, 38, 15), CODE(0, 37, 15),
    CODE(0, 36, 15), CODE(0, 35, 15), CODE(0, 34, 15), CODE(0, 33, 15),
    CODE(0, 32, 15), CODE(1, 14, 15), CODE(1, 13, 15), CODE(1, 12, 15),
    CODE(1, 11, 15), CODE(1, 10, 15), CODE(1, 9, 15), CODE(1, 8, 15)
};


// Table B-14/15, DCT coefficients table zero / one,
// codes 0000000000010000 ... 0000000000011111
const uint32_t PSXMDECDecoder::VLC_TABLE_6[16 * 2] =
{
    CODE(1, 18, 16), CODE(1, 17, 16), CODE(1, 16, 16), CODE(1, 15, 16),
    CODE(6, 3, 16), CODE(16, 2, 16), CODE(15, 2, 16), CODE(14, 2, 16),
    CODE(13, 2, 16), CODE(12, 2, 16), CODE(11, 2, 16), CODE(31, 1, 16),
    CODE(30, 1, 16), CODE(29, 1, 16), CODE(28, 1, 16), CODE(27, 1, 16)
};


const uint32_t PSXMDECDecoder::VLC_DC_Y_TABLE_0[48] =
{
    CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3),
    CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3),
    CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3),
    CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3),

    CODE1(0, -3, 4), CODE1(0, -3, 4), CODE1(0, -3, 4), CODE1(0, -3, 4),
    CODE1(0, -2, 4), CODE1(0, -2, 4), CODE1(0, -2, 4), CODE1(0, -2, 4),
    CODE1(0, 2, 4), CODE1(0, 2, 4), CODE1(0, 2, 4), CODE1(0, 2, 4),
    CODE1(0, 3, 4), CODE1(0, 3, 4), CODE1(0, 3, 4), CODE1(0, 3, 4),

    CODE1(0, 0, 3), CODE1(0, 0, 3), CODE1(0, 0, 3), CODE1(0, 0, 3),
    CODE1(0, 0, 3), CODE1(0, 0, 3), CODE1(0, 0, 3), CODE1(0, 0, 3),
    CODE1(0, -7, 6), CODE1(0, -6, 6), CODE1(0, -5, 6), CODE1(0, -4, 6),
    CODE1(0, 4, 6), CODE1(0, 5, 6), CODE1(0, 6, 6), CODE1(0, 7, 6),
};


const uint32_t PSXMDECDecoder::VLC_DC_UV_TABLE_0[56] =
{
    CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2),
    CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2),
    CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2),
    CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2), CODE1(0, 0, 2),

    CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3),
    CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3), CODE1(0, -1, 3),
    CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3),
    CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3), CODE1(0, 1, 3),

    CODE1(0, -3, 4), CODE1(0, -3, 4), CODE1(0, -3, 4), CODE1(0, -3, 4),
    CODE1(0, -2, 4), CODE1(0, -2, 4), CODE1(0, -2, 4), CODE1(0, -2, 4),
    CODE1(0, 2, 4), CODE1(0, 2, 4), CODE1(0, 2, 4), CODE1(0, 2, 4),
    CODE1(0, 3, 4), CODE1(0, 3, 4), CODE1(0, 3, 4), CODE1(0, 3, 4),

    CODE1(0, -7, 6), CODE1(0, -6, 6), CODE1(0, -5, 6), CODE1(0, -4, 6),
    CODE1(0, 4, 6), CODE1(0, 5, 6), CODE1(0, 6, 6), CODE1(0, 7, 6),
};


const uint32_t PSXMDECDecoder::VLC_ESCAPE_CODE = CODE1(63, 0, 6);
const uint32_t PSXMDECDecoder::VLC_EOB_CODE = CODE1(63, 512, 2);


const uint8_t PSXMDECDecoder::IQ_TABLE_Q_MATRIX[64] =
{
    2, 16, 19, 22, 26, 27, 29, 34,
    16, 16, 22, 24, 27, 29, 34, 37,
    19, 22, 26, 27, 29, 34, 34, 38,
    22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48,
    26, 27, 29, 32, 35, 40, 48, 58,
    26, 27, 29, 34, 38, 46, 56, 69,
    27, 29, 35, 38, 46, 56, 69, 83
};


// this table is based on djpeg by Independent Jpeg Group
const uint16_t PSXMDECDecoder::IQ_TABLE_AANSCALES_MATRIX[64] =
{
    // precomputed values scaled up by 14 bits
    16384, 22725, 21407, 19266, 16384, 12873, 8867, 4520,
    22725, 31521, 29692, 26722, 22725, 17855, 12299, 6270,
    21407, 29692, 27969, 25172, 21407, 16819, 11585, 5906,
    19266, 26722, 25172, 22654, 19266, 15137, 10426, 5315,
    16384, 22725, 21407, 19266, 16384, 12873, 8867, 4520,
    12873, 17855, 16819, 15137, 12873, 10114, 6967, 3552,
    8867, 12299, 11585, 10426, 8867, 6967, 4799, 2446,
    4520, 6270, 5906, 5315, 4520, 3552, 2446, 1247
};


const uint8_t PSXMDECDecoder::RL_ZSCAN_MATRIX[64] =
{
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};



PSXMDECDecoder::PSXMDECDecoder()
{
    BSRoundTableInit();
    IQTableInit();
}


void PSXMDECDecoder::BSRoundTableInit()
{
    for (uint16_t i = 0; i < 256; i++)
    {
        BSRoundTable[i] = 0;
        BSRoundTable[i + 256] = static_cast<uint8_t>(i);
        BSRoundTable[i + 512] = 255;
    }
}


void PSXMDECDecoder::IQTableInit()
{
    for (uint8_t i = 0; i < DCT_BLOCK_SIZE; i++)
        IQTable[i] = IQ_TABLE_Q_MATRIX[i] * IQ_TABLE_AANSCALES_MATRIX[i] >>
        (IQ_TABLE_CONST_BITS - IQ_TABLE_IFAST_SCALE_BITS);
}


void PSXMDECDecoder::IDCT(int16_t *arg_block, uint8_t arg_k)
{
    if (!arg_k)
    {
        int16_t val = (arg_block[0] >> (IDCT_PASS1_BITS + 3));
        for (uint8_t i = 0; i < DCT_BLOCK_SIZE; i++)
            arg_block[i] = val;

        return;
    }

    int16_t *ptr = arg_block;
    int16_t tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    int16_t z5, z10, z11, z12, z13;
    for (uint8_t i = 0; i < DCT_SIZE; i++, ptr++)
    {
        if ((ptr[DCT_SIZE * 1] | ptr[DCT_SIZE * 2] | ptr[DCT_SIZE * 3] |
            ptr[DCT_SIZE * 4] | ptr[DCT_SIZE * 5] | ptr[DCT_SIZE * 6] |
            ptr[DCT_SIZE * 7]) == 0)
        {
            ptr[DCT_SIZE * 0] = ptr[DCT_SIZE * 1] = ptr[DCT_SIZE * 2] =
                ptr[DCT_SIZE * 3] = ptr[DCT_SIZE * 4] = ptr[DCT_SIZE * 5] =
                ptr[DCT_SIZE * 6] = ptr[DCT_SIZE * 7] = ptr[DCT_SIZE * 0];

            continue;
        }

        z10 = ptr[DCT_SIZE * 0] + ptr[DCT_SIZE * 4];
        z11 = ptr[DCT_SIZE * 0] - ptr[DCT_SIZE * 4];
        z13 = ptr[DCT_SIZE * 2] + ptr[DCT_SIZE * 6];
        z12 = (((ptr[DCT_SIZE * 2] - ptr[DCT_SIZE * 6]) * IDCT_FIX_1_414213562)
            >> IDCT_CONST_BITS) - z13;

        tmp0 = z10 + z13;
        tmp3 = z10 - z13;
        tmp1 = z11 + z12;
        tmp2 = z11 - z12;

        z13 = ptr[DCT_SIZE * 3] + ptr[DCT_SIZE * 5];
        z10 = ptr[DCT_SIZE * 3] - ptr[DCT_SIZE * 5];
        z11 = ptr[DCT_SIZE * 1] + ptr[DCT_SIZE * 7];
        z12 = ptr[DCT_SIZE * 1] - ptr[DCT_SIZE * 7];

        z5 = (((z12 - z10) * IDCT_FIX_1_847759065) >> IDCT_CONST_BITS);
        tmp7 = z11 + z13;
        tmp6 = ((z10 * IDCT_FIX_2_613125930) >> IDCT_CONST_BITS) + z5 - tmp7;
        tmp5 = (((z11 - z13) * IDCT_FIX_1_414213562) >> IDCT_CONST_BITS) - tmp6;
        tmp4 = ((z12 * IDCT_FIX_1_082392200) >> IDCT_CONST_BITS) - z5 + tmp5;

        ptr[DCT_SIZE * 0] = (tmp0 + tmp7);
        ptr[DCT_SIZE * 7] = (tmp0 - tmp7);
        ptr[DCT_SIZE * 1] = (tmp1 + tmp6);
        ptr[DCT_SIZE * 6] = (tmp1 - tmp6);
        ptr[DCT_SIZE * 2] = (tmp2 + tmp5);
        ptr[DCT_SIZE * 5] = (tmp2 - tmp5);
        ptr[DCT_SIZE * 4] = (tmp3 + tmp4);
        ptr[DCT_SIZE * 3] = (tmp3 - tmp4);
    }

    ptr = arg_block;
    for (uint8_t i = 0; i < DCT_SIZE; i++, ptr += DCT_SIZE)
    {
        if ((ptr[1] | ptr[2] | ptr[3] | ptr[4] | ptr[5] | ptr[6] | ptr[7]) == 0)
        {
            ptr[0] = ptr[1] = ptr[2] = ptr[3] = ptr[4] = ptr[5] = ptr[6] =
                ptr[7] = (ptr[0] >> (IDCT_PASS1_BITS + 3));

            continue;
        }

        z10 = ptr[0] + ptr[4];
        z11 = ptr[0] - ptr[4];
        z13 = ptr[2] + ptr[6];
        z12 = (((ptr[2] - ptr[6]) * IDCT_FIX_1_414213562) >> IDCT_CONST_BITS) -
            z13;

        tmp0 = z10 + z13;
        tmp3 = z10 - z13;
        tmp1 = z11 + z12;
        tmp2 = z11 - z12;

        z13 = ptr[3] + ptr[5];
        z10 = ptr[3] - ptr[5];
        z11 = ptr[1] + ptr[7];
        z12 = ptr[1] - ptr[7];

        z5 = (((z12 - z10) * IDCT_FIX_1_847759065) >> IDCT_CONST_BITS);
        tmp7 = z11 + z13;
        tmp6 = ((z10 * IDCT_FIX_2_613125930) >> IDCT_CONST_BITS) + z5 - tmp7;
        tmp5 = (((z11 - z13) * IDCT_FIX_1_414213562) >> IDCT_CONST_BITS) - tmp6;
        tmp4 = ((z12 * IDCT_FIX_1_082392200) >> IDCT_CONST_BITS) - z5 + tmp5;

        ptr[0] = (tmp0 + tmp7) >> (IDCT_PASS1_BITS + 3);
        ptr[7] = (tmp0 - tmp7) >> (IDCT_PASS1_BITS + 3);
        ptr[1] = (tmp1 + tmp6) >> (IDCT_PASS1_BITS + 3);
        ptr[6] = (tmp1 - tmp6) >> (IDCT_PASS1_BITS + 3);
        ptr[2] = (tmp2 + tmp5) >> (IDCT_PASS1_BITS + 3);
        ptr[5] = (tmp2 - tmp5) >> (IDCT_PASS1_BITS + 3);
        ptr[4] = (tmp3 + tmp4) >> (IDCT_PASS1_BITS + 3);
        ptr[3] = (tmp3 - tmp4) >> (IDCT_PASS1_BITS + 3);
    }
}


void PSXMDECDecoder::DecodeDCTVLC(uint16_t *arg_mdec_rl,
    uint16_t *arg_mdec_bs)
{
    *(int32_t *)arg_mdec_rl = *(int32_t *)arg_mdec_bs;
    arg_mdec_rl += 2;

    uint16_t *rl_end = arg_mdec_rl + (uint16_t)arg_mdec_bs[0] * 2;
    uint16_t q_code = (arg_mdec_bs[2] << 10);
    uint16_t version = arg_mdec_bs[3];
    arg_mdec_bs += 4;

    uint32_t bitbuf = (arg_mdec_bs[0] << 16) | (arg_mdec_bs[1]);
    arg_mdec_bs += 2;

    int8_t incnt = -16;
    uint8_t n = 0;
    int16_t last_dc[3] = { 0, 0, 0 };
    while (arg_mdec_rl < rl_end)
    {
        uint32_t code2;

        // DC
        if (version < 3)
            code2 = (bitbuf >> (32 - 10)) | (10 << 16); // DC code
        else
        {
            code2 = (bitbuf >> (32 - 6));
            if (n >= 2)
            {
                // Y
                if (code2 < 48)
                {
                    code2 = VLC_DC_Y_TABLE_0[code2];
                    code2 = (code2 & 0xffff0000) | ((last_dc[2] +=
                        ((int16_t)(code2 << 6) >> 6) * 4) & 0x3ff);
                }
                else
                {
                    uint8_t bit = 3;
                    while ((bitbuf >> (32 - bit)) & 1)
                        bit++;
                    bit++;
                    uint8_t nbit = bit * 2 - 1;
                    int16_t val = (bitbuf >> (32 - nbit)) & ((1 << bit) - 1);
                    if ((val & (1 << (bit - 1))) == 0)
                        val -= (1 << bit) - 1;
                    val = (last_dc[2] += val * 4);
                    code2 = (nbit << 16) | (val & 0x3ff);
                }
            }
            else
            {
                // U,V
                if (code2 < 56)
                {
                    code2 = VLC_DC_UV_TABLE_0[code2];
                    code2 = (code2 & 0xffff0000) | ((last_dc[n] +=
                        ((int16_t)(code2 << 6) >> 6) * 4) & 0x3ff);
                }
                else
                {
                    uint8_t bit = 4;
                    while ((bitbuf >> (32 - bit)) & 1)
                        bit++;
                    uint8_t nbit = bit * 2;
                    int16_t val = (bitbuf >> (32 - nbit)) & ((1 << bit) - 1);
                    if ((val & (1 << (bit - 1))) == 0)
                        val -= (1 << bit) - 1;
                    val = (last_dc[n] += val * 4);
                    code2 = (nbit << 16) | (val & 0x3ff);
                }
            }
            if (++n == 6)
                n = 0;
        }
        code2 |= q_code;

        // AC
        for (;;)
        {
            *arg_mdec_rl++ = static_cast<uint16_t>(code2);
            bitbuf <<= (code2 >> 16);
            incnt += static_cast<int8_t>(code2 >> 16);
            while (incnt >= 0)
            {
                bitbuf |= *arg_mdec_bs++ << incnt;
                incnt -= 16;
            }

            code2 = (bitbuf >> (32 - VLC_SBIT));
            if (code2 >= 1 << (VLC_SBIT - 2))
            {
                code2 = VLC_TABLE_NEXT[(code2 >> 12) - 8];
                if (code2 == VLC_EOB_CODE)
                    break;
            }
            else if (code2 >= 1 << (VLC_SBIT - 6))
            {
                code2 = VLC_TABLE_0[(code2 >> 8) - 8];
                if (code2 == VLC_ESCAPE_CODE)
                {
                    bitbuf <<= 6;
                    incnt += 6;
                    while (incnt >= 0)
                    {
                        bitbuf |= *arg_mdec_bs++ << incnt;
                        incnt -= 16;
                    }
                    code2 = (bitbuf >> (32 - 16)) | (16 << 16);
                }
            }
            else if (code2 >= 1 << (VLC_SBIT - 7))
                code2 = VLC_TABLE_1[(code2 >> 6) - 16];
            else if (code2 >= 1 << (VLC_SBIT - 8))
                code2 = VLC_TABLE_2[(code2 >> 4) - 32];
            else if (code2 >= 1 << (VLC_SBIT - 9))
                code2 = VLC_TABLE_3[(code2 >> 3) - 32];
            else if (code2 >= 1 << (VLC_SBIT - 10))
                code2 = VLC_TABLE_4[(code2 >> 2) - 32];
            else if (code2 >= 1 << (VLC_SBIT - 11))
                code2 = VLC_TABLE_5[(code2 >> 1) - 32];
            else if (code2 >= 1 << (VLC_SBIT - 12))
                code2 = VLC_TABLE_6[(code2 >> 0) - 32];
            else
            {
                do
                {
                    *arg_mdec_rl++ = VLC_EOB;
                } while (arg_mdec_rl < rl_end);

                return;
            }
        }
        *arg_mdec_rl++ = static_cast<uint16_t>(code2); // EOB code
        bitbuf <<= 2;
        incnt += 2;
        while (incnt >= 0)
        {
            bitbuf |= *arg_mdec_bs++ << incnt;
            incnt -= 16;
        }
    }
}


uint16_t *PSXMDECDecoder::RL2BLK(uint16_t *arg_mdec_rl, int16_t *arg_blk)
{
    memset(arg_blk, 0, 6 * DCT_BLOCK_SIZE * sizeof(int16_t));

    for (uint8_t i = 0; i < 6; i++)
    {
        uint16_t rl = *arg_mdec_rl++;
        uint8_t q_scale = (rl >> 10);
        arg_blk[0] = static_cast<int16_t>(IQTable[0] * ((int16_t)(rl << 6) >> 6));
        uint8_t k = 0;
        for (;;)
        {
            rl = *arg_mdec_rl++;

            if (rl == VLC_EOB)
                break;
            k += (rl >> 10) + 1;
            arg_blk[RL_ZSCAN_MATRIX[k]] = static_cast<int16_t>(IQTable[RL_ZSCAN_MATRIX[k]] * q_scale * ((int16_t)(rl << 6) >> 6) / 8);
        }
        IDCT(arg_blk, k + 1);
        arg_blk += DCT_BLOCK_SIZE;
    }

    return arg_mdec_rl;
}

// An overly used bit of code in the YUV2BGRA32 function. Instead of huge code repeats, this will
// make things much more nicer.
void PSXMDECDecoder::YUVfunction1(uint8_t arg_image[][4], int index, int r0, int g0, int b0, int y)
{
    const int red = 2;
    const int green = 1;
    const int blue = 0;
    const int alpha = 3;

    arg_image[index][red] = BSRoundTable[r0 + y + 256];
    arg_image[index][green] = BSRoundTable[g0 + y + 256];
    arg_image[index][blue] = BSRoundTable[b0 + y + 256];
    arg_image[index][alpha] = 0xFF;
}

// Old code was a mess. New code should be much better.
// Could be cleaned up even more, but theres no need at the moment.
void PSXMDECDecoder::YUV2BGRA32(int16_t *arg_blk,
    uint8_t arg_image[][4])
{
    f64 rConstant = 1.402;
    f64 gConstant = -0.3437;
    f64 g2Constant = -0.7143;
    f64 bConstant = 1.772;

    int16_t *yblk = arg_blk + DCT_BLOCK_SIZE * 2;
    for (uint8_t yy = 0; yy < 16; yy += 2, arg_blk += 4, yblk += 8,
        arg_image += 24)
    {
        if (yy == 8)
            yblk += DCT_BLOCK_SIZE;
        for (uint8_t x = 0; x < 4; x++, arg_blk++, yblk += 2, arg_image += 2)
        {
            int16_t r0, g0, b0;

            // Set up YUV stuff
            r0 = static_cast<int16_t>(arg_blk[DCT_BLOCK_SIZE] * rConstant);
            g0 = static_cast<int16_t>((arg_blk[0] * gConstant) + (arg_blk[DCT_BLOCK_SIZE] * g2Constant));
            b0 = static_cast<int16_t>(arg_blk[0] * bConstant);

            int16_t y = yblk[0] + 128;
            YUVfunction1(arg_image, 0, r0, g0, b0, y);

            y = yblk[1] + 128;
            YUVfunction1(arg_image, 1, r0, g0, b0, y);

            y = yblk[8] + 128;
            YUVfunction1(arg_image, 16, r0, g0, b0, y);

            y = yblk[9] + 128;
            YUVfunction1(arg_image, 17, r0, g0, b0, y);


            // Set up YUV stuff again
            r0 = static_cast<int16_t>(arg_blk[4 + DCT_BLOCK_SIZE] * rConstant);
            g0 = static_cast<int16_t>((arg_blk[4] * gConstant) + (arg_blk[4 + DCT_BLOCK_SIZE] * g2Constant));
            b0 = static_cast<int16_t>(arg_blk[4] * bConstant);

            y = yblk[DCT_BLOCK_SIZE + 0] + 128;
            YUVfunction1(arg_image, 8, r0, g0, b0, y);

            y = yblk[DCT_BLOCK_SIZE + 1] + 128;
            YUVfunction1(arg_image, 9, r0, g0, b0, y);

            y = yblk[DCT_BLOCK_SIZE + 8] + 128;
            YUVfunction1(arg_image, 24, r0, g0, b0, y);

            y = yblk[DCT_BLOCK_SIZE + 9] + 128;
            YUVfunction1(arg_image, 25, r0, g0, b0, y);
        }
    }
}

uint8_t PSXMDECDecoder::DecodeFrameToABGR32(uint16_t *arg_decoded_image,
    uint16_t *arg_bs_image,
    uint16_t arg_width,
    uint16_t arg_height)
{
    uint16_t *rl = new uint16_t[(arg_bs_image[0] + 2) * sizeof(int32_t)];
    DecodeDCTVLC(rl, arg_bs_image);

    uint16_t *tmp_rl = rl;
    tmp_rl += 2;

    uint8_t color_bytes = 4;
    uint8_t w = color_bytes * 8;
    uint16_t height2 = (arg_height + 15) &~15;
    uint16_t *image = new uint16_t[height2 * w * sizeof(int16_t)];
    uint16_t slice = height2 * w / 2;
    arg_width = arg_width * color_bytes / 2;
    for (uint16_t x = 0; x < arg_width; x += w)
    {
        uint16_t *arg_image = image;
        uint16_t arg_size = slice;
        int16_t blk[DCT_BLOCK_SIZE * 6];
        uint16_t blocksize = 16 * 16 * color_bytes / 2;
        for (; arg_size > 0; arg_size -= blocksize / 2, arg_image += blocksize)
        {
            tmp_rl = RL2BLK(tmp_rl, blk);
            YUV2BGRA32(blk, (uint8_t(*)[4])arg_image);
        }


        uint16_t *src = image;
        uint16_t *dst = arg_decoded_image + x;
        for (int16_t y = arg_height - 1; y >= 0; y--)
        {
            memcpy(dst, src, w * 2);
            src += w;
            dst += arg_width;
        }
    }
    delete[] image;
    delete[] rl;

    return 0;
}
