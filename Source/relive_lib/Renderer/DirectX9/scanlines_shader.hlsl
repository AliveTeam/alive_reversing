//float2 fsTexSize : register(c12);
sampler texTextureData : register(s4);


uint ShiftLeft(uint value, uint bits)
{
    return value * pow(2, bits);
}

uint ShiftRight(uint value, uint bits)
{
    return value / pow(2, bits);
}

uint get888FromNormalized(float r, float g, float b)
{
    uint rValue = r * 255.0f;
    uint gValue = g * 255.0f;
    uint bValue = b * 255.0f;

    rValue = rValue * pow(2, 16); // 2^ 16;
    gValue = gValue * pow(2, 8); // 2^ 8;

    return rValue + gValue + bValue;
}

float3 getNormalizedFrom888(uint rgbInput)
{
    // TODO: this is wrong when not an int, int casts go over the inst limits
    float t1 = rgbInput / pow(2, 16);
    float t2 = rgbInput * pow(2, 16);
    float t3 = rgbInput * pow(2, 24);
    float t4 = ((uint)t2) / pow(2, 24);
    float t5 = ((uint)t3) / pow(2, 24);

    float rValue = float(t1) / 255.0f;
    float gValue = float(t4) / 255.0f;
    float bValue = float(t5) / 255.0f;


    return float3(rValue, gValue, bValue);
}

uint Roll24Bits(uint value)
{
    // value & 1

    // clears bit 0
    uint valueShiftedRight = ShiftRight(value, 1);
    uint temp = ShiftLeft(value, 31);
    uint valueAnd1 = ShiftRight(temp, 31);

    // valueShiftedRight |= valueAnd1 ? 0x800000 : 0x0;

    // Drop the bit to roll
        // If the bit we removed was set then set it on the
        // other end
        valueShiftedRight += (valueAnd1 * pow(2, 23)); //     0x800000;
    return valueShiftedRight;
}

uint ApplyF8Mask(uint v)
{
    // same v = v & 0xF8
    v = ShiftRight(v, 3);
    v = ShiftLeft(v, 3);
    return v;
}

uint ApplyMask(uint r, uint g, uint b)
{
    r = ApplyF8Mask(r);
    g = ApplyF8Mask(g);
    b = ApplyF8Mask(b);

    r = r * pow(2, 16); // 2^ 16;
    g = g * pow(2, 8); // 2^ 8;

    return r + g + b;
}

float4 PS(float4 fsShadeColor
          : COLOR0,
          float2 fsUV
          : TEXCOORD0) : COLOR
{
    float4 belowTexel = tex2D(texTextureData, fsUV);
    float4 aboveTexel = tex2D(texTextureData, float2(fsUV.x, fsUV.y + 1.0));

    bool scanline = false;//  fmod(fsUV[1] / fsTexSize[1], 2.0f) > 0; //  int(mod(gl_FragCoord.y, 2.0f)) > 0;

    if (scanline)
    {
        // Do the bit rotation stuff
        uint tmp1 = ApplyMask(abs(aboveTexel.r / 255.0f), abs(aboveTexel.g / 255.0f), abs(aboveTexel.b / 255.0f));
        uint tmp2 = ApplyMask(abs(belowTexel.r / 255.0f), abs(belowTexel.g / 255.0f), abs(belowTexel.b / 255.0f));
        uint pixelResult = Roll24Bits(tmp1 + tmp2);

        belowTexel.rgb = getNormalizedFrom888(pixelResult);
    }

    return belowTexel;
}
