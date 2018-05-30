/*
 * MLHE muxer
 * Copyright (c) 2000 Fabrice Bellard
 *
 * first version by Marina González <magonzalezc@dit.upm.es>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "avformat.h"
#include "internal.h"
#include "libavutil/avassert.h"
#include "libavutil/imgutils.h"
#include "libavutil/log.h"
#include "libavutil/opt.h"
#include "libavcodec/lhe.h"

typedef struct MlheContext {
    AVClass *class;
    AVPacket *prev_pkt;
    int duration;
    int last_delay;
} MlheContext;

static int mlhe_image_write_header(AVIOContext *pb, AVStream *st)
{
    avio_write(pb, "MLHE", 4);
    avio_wl16(pb, st->codecpar->width);
    avio_wl16(pb, st->codecpar->height);
    avio_w8(pb, st->codecpar->format);
    avio_wl16(pb, st->time_base.den);
    avio_wl16(pb, st->codec->time_base.den);

    avio_flush(pb);
    return 0;
}

static int mlhe_write_header(AVFormatContext *s)
{
    if (s->nb_streams != 1 ||
        s->streams[0]->codecpar->codec_type != AVMEDIA_TYPE_VIDEO ||
        s->streams[0]->codecpar->codec_id   != AV_CODEC_ID_MLHE) {
        av_log(s, AV_LOG_ERROR,
               "MLHE muxer supports only a single video MLHE stream.\n");
        return AVERROR(EINVAL);
    }

    mlhe_image_write_header(s->pb, s->streams[0]); 

    return 0;
}

static int flush_packet(AVFormatContext *s, AVPacket *new)
{
    MlheContext *mlhe = s->priv_data;
    AVIOContext *pb = s->pb;
    AVPacket *pkt = mlhe->prev_pkt;

    if (!pkt) 
        return 0;

    /* Control byte */
    avio_w8(pb, MLHE_EXTENSION_INTRODUCER);

    av_packet_unref(mlhe->prev_pkt);
    if (new) 
        av_copy_packet(mlhe->prev_pkt, new);
    
    avio_wl32(pb, pkt->size);
    avio_write(pb, pkt->data, pkt->size);

    return 0;
}

static int mlhe_write_packet(AVFormatContext *s, AVPacket *pkt)
{    
    int ret;
    
    MlheContext *mlhe = s->priv_data;
    
    if (!mlhe->prev_pkt) {
        mlhe->prev_pkt = av_malloc(sizeof(*mlhe->prev_pkt));
        if (!mlhe->prev_pkt)
            return AVERROR(ENOMEM);


        ret = av_copy_packet(mlhe->prev_pkt, pkt);
        if (ret<0) 
            return ret;
    }
    
    return flush_packet(s, pkt);
}

static int mlhe_write_trailer(AVFormatContext *s)
{
    MlheContext *mlhe = s->priv_data;
    AVIOContext *pb = s->pb;

    flush_packet(s, NULL);
    av_freep(&mlhe->prev_pkt);
    avio_w8(pb, MLHE_TRAILER);

    return 0;
}

#define OFFSET(x) offsetof(MlheContext, x)
#define ENC AV_OPT_FLAG_ENCODING_PARAM
static const AVOption options[] = {
    { NULL },
};

static const AVClass mlhe_muxer_class = {
    .class_name = "MLHE muxer",
    .item_name  = av_default_item_name,
    .version    = LIBAVUTIL_VERSION_INT,
    .option     = options,
};

AVOutputFormat ff_mlhe_muxer = {
    .name           = "mlhe",
    .long_name      = NULL_IF_CONFIG_SMALL("MLHE - LHE Video"),
    .mime_type      = "image/lhe",
    .extensions     = "mlhe",
    .priv_data_size = sizeof(MlheContext),
    .audio_codec    = AV_CODEC_ID_NONE,
    .video_codec    = AV_CODEC_ID_MLHE,
    .write_header   = mlhe_write_header,
    .write_packet   = mlhe_write_packet,
    .write_trailer  = mlhe_write_trailer,
    .priv_class     = &mlhe_muxer_class,
    .flags          = AVFMT_VARIABLE_FPS,
};
