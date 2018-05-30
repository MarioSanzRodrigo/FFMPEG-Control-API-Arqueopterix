/*
 * MLHE demuxer
 * Copyright (c) 2012 Vitaliy E Sugrobov
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

/**
 * @file
 * GIF demuxer.
 */

#include "avformat.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/opt.h"
#include "internal.h"
#include "libavcodec/lhe.h"

typedef struct MlheDemuxContext {
    const AVClass *class;
    int nb_frames;
    int last_duration;
    int delay;
} MlheDemuxContext;

static int mlhe_probe(AVProbeData *p)
{
    /* check magick */
    if (memcmp(p->buf, mlhe_sig, 4))
        return 0;

    /* width or height contains zero? */
    if (!AV_RL16(&p->buf[4]) || !AV_RL16(&p->buf[6]))
        return 0;

    return AVPROBE_SCORE_MAX;
}

static int resync(AVIOContext *pb)
{
    int i;
    for (i = 0; i < 4; i++) {
        int b = avio_r8(pb);
        if (b != mlhe_sig[i])
            i = -(b != 'M');
        if (avio_feof(pb))
            return AVERROR_EOF;
    }
    return 0;
}

static int mlhe_read_header(AVFormatContext *s)
{
    AVIOContext     *pb  = s->pb;
    AVStream        *st;
    int width, height, format, time_base_den, codec_time_base_den, ret;

    if ((ret = resync(pb)) < 0)
        return ret;

    width  = avio_rl16(pb);
    height = avio_rl16(pb);
    format = avio_r8(pb);
    time_base_den = avio_rl16 (pb);
    codec_time_base_den = avio_rl16(pb);
    
    if (width == 0 || height == 0)
        return AVERROR_INVALIDDATA;

    st = avformat_new_stream(s, NULL);
    if (!st)
        return AVERROR(ENOMEM);

    st->time_base.den = time_base_den;
    st->codec->time_base.den = codec_time_base_den;
    st->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    st->codecpar->codec_id   = AV_CODEC_ID_MLHE;
    st->codecpar->width      = width;
    st->codecpar->height     = height;
    st->codecpar->format     = format;
        
    return 0;
}

static int mlhe_read_packet(AVFormatContext *s, AVPacket *pkt)
{ 
    MlheDemuxContext *mlhe = s->priv_data;
    AVIOContext *pb = s->pb;
    int packed_fields, block_label, ct_size,
        keyframe, frame_parsed, ret;
    int64_t frame_start, frame_end;
    uint32_t pkt_size;
    unsigned char buf[6];
    int read;
    
    frame_parsed = 0;
        
    if ((ret = avio_read(pb, buf, 4)) == 4) {
        keyframe = memcmp(buf, mlhe_sig, 4) == 0;
        
    } else if (ret < 0) {
        return ret;
    } else {
        keyframe = 0;
    }
    

    if (keyframe) { 
        /* skip 2 bytes of width and 2 of height */
        if ((ret = avio_skip(pb, 4)) < 0)
            return ret;        
    } else {
        avio_seek(pb, -ret, SEEK_CUR);
        ret = AVERROR_EOF;
    }

    block_label = avio_r8(pb);
    
    
    while (MLHE_TRAILER != block_label && !avio_feof(pb)) {
        
        if (block_label == MLHE_EXTENSION_INTRODUCER) {  
                
            pkt_size = avio_rl32(pb);
                        
            frame_start = avio_tell(pb);
                        
            avio_skip(pb, pkt_size);           

            frame_end = avio_tell(pb);

            if (avio_seek(pb, frame_start, SEEK_SET) != frame_start)
                return AVERROR(EIO);

            ret = av_get_packet(pb, pkt, frame_end - frame_start);
            
            if (ret < 0)
                return ret;

            if (keyframe)
                pkt->flags |= AV_PKT_FLAG_KEY;

            pkt->stream_index = 0;
            pkt->duration = 1;
            
            mlhe->nb_frames ++;
            mlhe->last_duration = pkt->duration;

            frame_parsed = 1;

            break;
        }
    }

    if ((ret >= 0 && !frame_parsed) || ret == AVERROR_EOF) {
        if (mlhe->nb_frames == 1) {
            s->streams[0]->r_frame_rate = (AVRational) {100, mlhe->last_duration};
        }

        return AVERROR_EOF;
    } else
        return ret;
}

static const AVOption options[] = {
    { NULL },
};

static const AVClass demuxer_class = {
    .class_name = "MLHE demuxer",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
    .category   = AV_CLASS_CATEGORY_DEMUXER,
};

AVInputFormat ff_mlhe_demuxer = {
    .name           = "mlhe",
    .long_name      = NULL_IF_CONFIG_SMALL("MLHE - LHE Video"),
    .priv_data_size = sizeof(MlheDemuxContext),
    .read_probe     = mlhe_probe,
    .read_header    = mlhe_read_header,
    .read_packet    = mlhe_read_packet,
    .flags          = AVFMT_GENERIC_INDEX,
    .priv_class     = &demuxer_class,
};
