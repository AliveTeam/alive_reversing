#include "fmv_converter.hpp"
#include "../../AliveLibAE/PathData.hpp"
#include "../../AliveLibCommon/FatalError.hpp"

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4505)
#endif
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

static int write_frame(FILE* outFile, const uint8_t* buffer, size_t size /*, int64_t pts*/)
{
    //ivf_write_frame_header(writer->file, pts, size);
    if (fwrite(buffer, 1, size, outFile) != size)
    {
        return 0;
    }

    //++writer->frame_count;

    return 1;
}
static bool encode_frame(aom_codec_ctx_t* codec, aom_image_t* img, int frame_index, int flags , FILE* writer)
{
    bool got_pkts = false;
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt = nullptr;
    const aom_codec_err_t res = aom_codec_encode(codec, img, frame_index, 1, flags);
    if (res != AOM_CODEC_OK)
    {
        ALIVE_FATAL("Failed to encode frame");
    }

    while ((pkt = aom_codec_get_cx_data(codec, &iter)) != nullptr)
    {
        got_pkts = true;

        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT)
        {
            const int keyframe = (pkt->data.frame.flags & AOM_FRAME_IS_KEY) != 0;
            if (!write_frame(writer, static_cast<const uint8_t*>(pkt->data.frame.buf), pkt->data.frame.sz /* pkt->data.frame.pts*/))
            {
                ALIVE_FATAL("Failed to write compressed frame");
            }
            LOG_INFO(keyframe ? "K" : ".");
        }
    }

    return got_pkts;
}

struct VideoInfo final
{
    u32 width = 0;
    u32 height = 0;
};

void ConvertFMVs(const FileSystem::Path& /*dataDir*/, bool isAo)
{
    // TODO: Conversion
    if (!isAo)
    {
        VideoInfo info;
        info.width = 320;
        info.height = 240;

        FmvInfo* pInfo = Path_Get_FMV_Record(EReliveLevelIds::eMines, 1);
        if (pInfo)
        {
            aom_codec_iface_t* encoder = &aom_codec_av1_cx_algo;
            if (!encoder)
            {
                ALIVE_FATAL("Unsupported codec.");
            }

            aom_image_t raw;
            if (!aom_img_alloc(&raw, AOM_IMG_FMT_YV12, info.width, info.height, 1))
            {
                ALIVE_FATAL("Failed to allocate image.");
            }

            aom_codec_enc_cfg_t cfg = {};
            if (aom_codec_enc_config_default(encoder, &cfg, AOM_USAGE_GOOD_QUALITY))
            {
                ALIVE_FATAL("Failed to get default codec config.");
            }
            cfg.g_w = info.width;
            cfg.g_h = info.height;
            //cfg.g_timebase.num = info.time_base.numerator;
            //cfg.g_timebase.den = info.time_base.denominator;
            //cfg.rc_target_bitrate = bitrate;

            /*
            writer = aom_video_writer_open(outfile_arg, kContainerIVF, &info);
            if (!writer)
            {
                die("Failed to open %s for writing.", outfile_arg);
            }*/

            aom_codec_ctx_t codec = {};
            if (aom_codec_enc_init(&codec, encoder, &cfg, 0))
            {
                ALIVE_FATAL("Failed to initialize encoder");
            }

            const int speed = 2;
            if (aom_codec_control(&codec, AOME_SET_CPUUSED, speed))
            {
                ALIVE_FATAL("Failed to set cpu-used");
            }

            const u32 keyframe_interval = 30;
            u32 frame_count = 0;
            u32 frames_encoded = 0;
            u32 max_frames = 0; // TODO

            FILE* outFile = fopen("test.ivf", "wb");
            if (!outFile)
            {
                ALIVE_FATAL("File to open output file");
            }

            // TODO: read yuv frame
            for (u32 i=0; i<20; i++)
            {
                int flags = 0;
                if (keyframe_interval > 0 && frame_count % keyframe_interval == 0)
                {
                    flags |= AOM_EFLAG_FORCE_KF;
                }

                encode_frame(&codec, &raw, frame_count++, flags, outFile);
                frames_encoded++;
                if (max_frames > 0 && frames_encoded >= max_frames)
                {
                    //break;
                }
            }

            // Flush encoder.
            while (encode_frame(&codec, NULL, -1, 0, outFile))
            {
                continue;
            }

            fclose(outFile);

            aom_img_free(&raw);
        }
    }
}
