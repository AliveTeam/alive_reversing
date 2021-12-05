#include "Base64.hpp"

namespace ReliveAPI {
static const unsigned char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64_encode(const u8* src, size_t len)
{
    unsigned char *out, *pos;
    const unsigned char *end, *in;

    size_t olen = 4 * ((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len)
    {
        return std::string(); /* integer overflow */
    }

    std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*) &outStr[0];

    end = src + len;
    in = src;
    pos = out;
    while (end - in >= 3)
    {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
    }

    if (end - in)
    {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1)
        {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else
        {
            *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return outStr;
}

static const int B64index[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
                                  56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6,
                                  7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,
                                  0, 0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

static std::vector<u8> b64decode(const u8* data, const size_t len)
{
    const u8* p = data;
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t calcLen = ((len + 3) / 4 - pad) * 4;
    std::vector<u8> str(calcLen / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < calcLen; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = (n >> 16) & 0xFF;
        str[j++] = (n >> 8) & 0xFF;
        str[j++] = n & 0xFF;
    }

    if (pad)
    {
        int n = B64index[p[calcLen]] << 18 | B64index[p[calcLen + 1]] << 12;
        str[str.size() - 1] = (n >> 16) & 0xFF;

        if (len > calcLen + 2 && p[calcLen + 2] != '=')
        {
            n |= B64index[p[calcLen + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }

    return str;
}

std::string ToBase64(const std::vector<u8>& vec)
{
    return base64_encode(vec.data(), vec.size());
}

std::vector<u8> FromBase64(const std::string& vec)
{
    return b64decode(reinterpret_cast<const u8*>(vec.data()), vec.length());
}
} // namespace ReliveAPI
