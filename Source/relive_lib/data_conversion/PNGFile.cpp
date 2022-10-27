#include "PNGFile.hpp"
#include "rgb_conversion.hpp"
#include "file_system.hpp"
#include "../../AliveLibCommon/logger.hpp"
#include "../../AliveLibCommon/FatalError.hpp"
#include "AnimationConverter.hpp"
#include <spng/spng.h>

class PngContext final
{
public:
    enum class CtxType
    {
        Decode,
        Encode,
    };

    explicit PngContext(CtxType type)
    {
        ctx = spng_ctx_new(type == CtxType::Encode ? SPNG_CTX_ENCODER : 0);
    }

    ~PngContext()
    {
        if (ctx)
        {
            spng_ctx_free(ctx);
        }
    }

    spng_ctx* ctx = nullptr;
};

class PngApi final
{
public:
    explicit PngApi(PngContext::CtxType ctx)
        : mCtx(ctx)
    {

    }

    void DisableCRC()
    {
        // Don't calc CRCs
        s32 ret = spng_set_crc_action(mCtx.ctx, SPNG_CRC_USE, SPNG_CRC_USE);
        if (ret)
        {
            ALIVE_FATAL("spng_set_crc_action failed");
        }
    }

    void SetFile(FILE* file)
    {
        s32 ret = spng_set_png_file(mCtx.ctx, file);
        if (ret)
        {
            ALIVE_FATAL("spng_set_png_file failed");
        }
    }

    void LimitChunkSize()
    {
        // Memory limit for unknown chunks
        const std::size_t limit = 1024 * 1024 * 64;
        s32 ret = spng_set_chunk_limits(mCtx.ctx, limit, limit);
        if (ret)
        {
            ALIVE_FATAL("spng_set_chunk_limits failed");
        }
    }

    spng_ihdr GetHeader()
    {
        spng_ihdr ihdr;
        s32 ret = spng_get_ihdr(mCtx.ctx, &ihdr);
        if (ret)
        {
            LOG_ERROR("spng_get_ihdr() error: " << spng_strerror(ret));
            ALIVE_FATAL("get png header failed");
        }
        return ihdr;
    }

    void SetHeader(spng_ihdr& hdr)
    {
        s32 ret = spng_set_ihdr(mCtx.ctx, &hdr);
        if (ret)
        {
            ALIVE_FATAL("spng_set_ihdr failed");
        }

    }

    spng_plte GetPal()
    {
        // Real pal, if any
        spng_plte plte = {};
        s32 ret = spng_get_plte(mCtx.ctx, &plte);
        if (ret)
        {
            if (ret == SPNG_ECHUNKAVAIL)
            {
                LOG_ERROR("PNG has no pal (not an indexed png");
                ALIVE_FATAL("PNG is not indexed");
            }
            ALIVE_FATAL("Failed to load png pal");
        }
        return plte;
    }

    void SetPal(spng_plte& pal)
    {
        s32 ret = spng_set_plte(mCtx.ctx, &pal);
        if (ret)
        {
            ALIVE_FATAL("spng_set_plte failed");
        }
    }

    spng_trns GetTrans()
    {
        spng_trns trns = {};
        s32 ret = spng_get_trns(mCtx.ctx, &trns);
        if (ret && ret != SPNG_ECHUNKAVAIL)
        {
            ALIVE_FATAL("spng_get_trns failed");
        }
        return trns;
    }

    void SetTrans(spng_trns& trans)
    {
        s32 ret = spng_set_trns(mCtx.ctx, &trans);
        if (ret)
        {
            ALIVE_FATAL("spng_set_trns failed");
        }
    }

    std::size_t ImageSize()
    {
        std::size_t image_size = 0;
        s32 ret = spng_decoded_image_size(mCtx.ctx, SPNG_FMT_PNG, &image_size);
        if (ret)
        {
            ALIVE_FATAL("Failed to get decoded png size");
        }
        return image_size;
    }

    PngContext mCtx;
};

static int spng_rw_fn_impl(spng_ctx* /*ctx*/, void* user, void* dst_src, size_t length)
{
    PNGFile* ptr = (PNGFile*) user;
   
    if (ptr->mWrite)
    {
        for (size_t i = 0; i < length; i++)
        {
            ptr->mOutBuffer.push_back(((u8*) dst_src)[i]);
        }
    }
    else
    {
        memcpy(dst_src, ptr->mPtr, length);
        ptr->mPtr += static_cast<u32>(length);
    }

    return 0;
}

std::vector<u8> PNGFile::Encode(const u32* pixelData, u32 width, u32 height)
{
    PngApi api(PngContext::CtxType::Encode);

    mWrite = true;
    mOutBuffer.clear();
    mOffset = 0;
    mPtr = (u8*)reinterpret_cast<const u8*>(pixelData);
    spng_set_png_stream(api.mCtx.ctx, spng_rw_fn_impl, this);

    spng_ihdr header = {};
    header.width = width;
    header.height = height;
    header.color_type = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    header.bit_depth = 8;
    api.SetHeader(header);

    std::size_t len = api.ImageSize();

    s32 ret = spng_encode_image(api.mCtx.ctx, pixelData, len, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    if (ret)
    {
        ALIVE_FATAL("spng_encode_image failed");
    }


    return this->mOutBuffer;
}


void PNGFile::Decode(const std::vector<u8>& pngData, std::vector<u8>& rawPixels, u32& width, u32& height)
{
    PngApi api(PngContext::CtxType::Decode);

    mWrite = false;
    mOutBuffer.clear();
    mOffset = 0;
    mPtr = (u8*)pngData.data();

    spng_set_png_stream(api.mCtx.ctx, spng_rw_fn_impl, this);

    spng_ihdr header = api.GetHeader();
    width = header.width;
    height = header.height;

    std::size_t image_size = api.ImageSize();
    rawPixels.resize(image_size);
    s32 ret = spng_decode_image(api.mCtx.ctx, rawPixels.data(), rawPixels.size(), SPNG_FMT_PNG, 0);
    if (ret)
    {
        ALIVE_FATAL("spng_decode_image failed");
    }
}

void PNGFile::Load(const char_type* pFileName, std::vector<u8>& pixelData, u32& width, u32& height)
{
    FileSystem fs;
    std::vector<u8> buffer;
    fs.LoadToVec(pFileName, buffer);
    Decode(buffer, pixelData, width, height);
}

void PNGFile::Load(const char_type* pFileName, AnimationPal& pal256, std::vector<u8>& pixelData, u32& width, u32& height)
{
    AutoFILE f;
    f.Open(pFileName, "rb", false);

    PngApi api(PngContext::CtxType::Decode);

    api.DisableCRC();
    api.LimitChunkSize();
    api.SetFile(f.GetFile());

    const spng_ihdr ihdr = api.GetHeader();
    width = ihdr.width;
    height = ihdr.height;

    spng_plte plte = api.GetPal();
    spng_trns trns = api.GetTrans();

    for (u32 i = 0; i < plte.n_entries; i++)
    {
        u8 alpha = 0;
        if (i < trns.n_type3_entries)
        {
            alpha = trns.type3_alpha[i];
        }
        pal256.mPal[i] = RGBConversion::RGB888ToRGB555(plte.entries[i].red, plte.entries[i].green, plte.entries[i].blue, alpha);
    }

    if (ihdr.color_type != SPNG_COLOR_TYPE_INDEXED)
    {
        ALIVE_FATAL("PNG is not indexed");
    }

    std::size_t image_size = api.ImageSize();

    pixelData.resize(image_size);
    s32 ret = spng_decode_image(api.mCtx.ctx, pixelData.data(), pixelData.size(), SPNG_FMT_PNG, 0);
    if (ret)
    {
        ALIVE_FATAL("spng_decode_image failed");
    }
}

void PNGFile::Save(const char_type* pFileName, const AnimationPal& pal256, const std::vector<u8>& pixelData, u32 width, u32 height)
{
    // The TGA header uses a var length id string which means we can't just use
    // a struct to represent it since the alignment is not fixed until after this field.
    AutoFILE f;
    f.Open(pFileName, "wb", false);

    PngApi api(PngContext::CtxType::Encode);

    api.SetFile(f.GetFile());

    spng_ihdr ihdr = {};
    ihdr.width = width;
    ihdr.height = height;
    ihdr.color_type = SPNG_COLOR_TYPE_INDEXED;
    ihdr.bit_depth = 8;

    api.SetHeader(ihdr);

    spng_plte plte = {};
    spng_trns trns = {};
    trns.n_type3_entries = 255;
    plte.n_entries = 255;
    for (u32 i = 0; i < 255; i++)
    {
        const RGBConversion::RGBA32 rgb = RGBConversion::RGBA555ToRGBA888Components(pal256.mPal[i]);
        plte.entries[i].red = rgb.r;
        plte.entries[i].green = rgb.g;
        plte.entries[i].blue = rgb.b;
        trns.type3_alpha[i] = rgb.a;
    }

    api.SetPal(plte);
    api.SetTrans(trns);

    s32 ret = spng_encode_image(api.mCtx.ctx, pixelData.data(), pixelData.size(), SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    if (ret)
    {
        ALIVE_FATAL("spng_encode_image failed");
    }
}
