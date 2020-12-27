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



#ifndef PSX_MDEC_DECODER_H__
#define PSX_MDEC_DECODER_H__



#include <stdint.h>



class PSXMDECDecoder
{
public:
    PSXMDECDecoder();

    uint8_t DecodeFrameToABGR32(uint16_t *arg_decoded_image,
        uint16_t *arg_bs_image,
        uint16_t arg_width,
        uint16_t arg_height);

    static void IDCT(int16_t *, uint8_t);
private:
    static const uint8_t  VLC_SBIT = 17;
    static const uint16_t VLC_EOB = 0xfe00;
    static const uint32_t VLC_ESCAPE_CODE;
    static const uint32_t VLC_EOB_CODE;
    static const uint32_t VLC_TABLE_NEXT[12 * 2];
    static const uint32_t VLC_TABLE_0[60 * 2];
    static const uint32_t VLC_TABLE_1[8 * 2];
    static const uint32_t VLC_TABLE_2[16 * 2];
    static const uint32_t VLC_TABLE_3[16 * 2];
    static const uint32_t VLC_TABLE_4[16 * 2];
    static const uint32_t VLC_TABLE_5[16 * 2];
    static const uint32_t VLC_TABLE_6[16 * 2];
    static const uint32_t VLC_DC_Y_TABLE_0[48];
    static const uint32_t VLC_DC_UV_TABLE_0[56];

    static const uint8_t DCT_SIZE = 8;
    static const uint8_t DCT_BLOCK_SIZE = 64;

    static const uint8_t  IDCT_CONST_BITS = 8;
    static const uint8_t  IDCT_PASS1_BITS = 2;
    static const uint16_t IDCT_FIX_1_082392200 = 277;
    static const uint16_t IDCT_FIX_1_414213562 = 362;
    static const uint16_t IDCT_FIX_1_847759065 = 473;
    static const uint16_t IDCT_FIX_2_613125930 = 669;

    static const uint8_t  IQ_TABLE_CONST_BITS = 14;
    static const uint8_t  IQ_TABLE_IFAST_SCALE_BITS = 2;
    static const uint8_t  IQ_TABLE_Q_MATRIX[DCT_BLOCK_SIZE];
    static const uint16_t IQ_TABLE_AANSCALES_MATRIX[DCT_BLOCK_SIZE];

    static const uint8_t RL_ZSCAN_MATRIX[DCT_BLOCK_SIZE];

    uint8_t BSRoundTable[256 * 3];
    int IQTable[DCT_BLOCK_SIZE];

    void BSRoundTableInit();
    void IQTableInit();
   
    void YUVfunction1(uint8_t arg_image[][4], int index, int r0, int g0, int b0, int y);
    void YUV2BGRA32(int16_t *arg_blk,
        uint8_t arg_image[][4]);

    uint16_t *RL2BLK(uint16_t *, int16_t *);
    void DecodeDCTVLC(uint16_t *mdec_rl, uint16_t *mdec_bs);
};



#endif //PSX_MDEC_DECODER_H__
