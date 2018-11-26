/*
Clase encargada de crear paquetes con frames
*/


#include "Encoder.h"
#include <sys/time.h>

namespace payloader {

DEFINE_LOGGER(Encoder, "Encoder");

Encoder::Encoder() {
	sink_ = NULL;
    vEncoder_ = NULL;
    vEncoderContext_ = NULL;
    aEncoder_ = NULL;
    aEncoderContext_ = NULL;
}

Encoder::~Encoder() {

}

int Encoder::init(AudioCodecInfo audioInfo, VideoCodecInfo videoInfo){

	avcodec_register_all();

	if (videoInfo.enabled) {
		vEncoder_ = avcodec_find_encoder(videoInfo.codec);
		if (!vEncoder_) {
            ELOG_DEBUG("Error getting video encoder");
            return -1;
        }
        vEncoderContext_ = avcodec_alloc_context3(vEncoder_);
        if (!vEncoderContext_) {
            ELOG_DEBUG("Error getting allocating decoder context");
            return -1;
        }

        vEncoderContext_->bit_rate = videoInfo.bitRate;
	    vEncoderContext_->width = videoInfo.width;
	    vEncoderContext_->height = videoInfo.height;
	    vEncoderContext_->time_base = (AVRational){1, 25};
	    vEncoderContext_->framerate = (AVRational){25, 1};
	    vEncoderContext_->gop_size = 10;
	    vEncoderContext_->max_b_frames=1;
	    vEncoderContext_->pix_fmt = AV_PIX_FMT_YUV420P;
	    
	    if (avcodec_open2(vEncoderContext_, vEncoder_, NULL) < 0) {
	        ELOG_DEBUG("Error opening codec");
            return -1;
	    }
	}

	if (audioInfo.enabled) {
		aEncoder_ = avcodec_find_encoder(audioInfo.codec);
		if (!aEncoder_) {
            ELOG_DEBUG("Error getting video encoder");
            return -1;
        }
        aEncoderContext_ = avcodec_alloc_context3(aEncoder_);
        if (!aEncoderContext_) {
            ELOG_DEBUG("Error getting allocating decoder context");
            return -1;
        }

	    aEncoderContext_->sample_fmt = AV_SAMPLE_FMT_S16;
        aEncoderContext_->sample_rate = 48000;
        aEncoderContext_->channels = 2;

	    if (avcodec_open2(aEncoderContext_, aEncoder_, NULL) < 0) {
	        ELOG_DEBUG("Error opening codec");
            return -1;
	    }
	}



    return true;

}

void Encoder::setSink(PacketReceiver* receiver) {
	sink_ = receiver;
}

/*
Crea un paquete con las tramas
*/

void Encoder::receiveFrame(AVFrame* frame, AVMediaType type) {

	if (type == AVMEDIA_TYPE_VIDEO) {

		if (vEncoder_ == 0 || vEncoderContext_ == 0) {
            ELOG_DEBUG("Init Codec First");
        }

		ELOG_DEBUG("Received video frame %d", frame->width);

		AVPacket *pkt;
		pkt = av_packet_alloc();
	    if (!pkt)
	        ELOG_DEBUG("Error alloc packet");

		int ret;

	    ret = avcodec_send_frame(vEncoderContext_, frame);
	    if (ret < 0) {
	        ELOG_DEBUG("error sending a frame for encoding");
	    }
	    while (ret >= 0) {
	        ret = avcodec_receive_packet(vEncoderContext_, pkt);
	        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
	            return;
	        else if (ret < 0) {
	            ELOG_DEBUG("error during encoding\n");
	            return;
	        }
	        ELOG_DEBUG("encoded frame %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
	        
	        if (sink_ != NULL) {
	            sink_->receivePacket(*pkt, AVMEDIA_TYPE_VIDEO);
	        }
	        av_packet_unref(pkt);
	    }

	} else if (type == AVMEDIA_TYPE_AUDIO) {
        if (aEncoder_ == 0 || aEncoderContext_ == 0) {
            ELOG_DEBUG("Init Codec First");
        }

		ELOG_DEBUG("Received audio frame %d", frame->nb_samples);

		AVPacket *pkt;
		pkt = av_packet_alloc();
	    if (!pkt)
	        ELOG_DEBUG("Error alloc packet");

		int ret;

	    ret = avcodec_send_frame(aEncoderContext_, frame); // es igual que avcodec_encode_video2¿¿ mirar decoder
	    if (ret < 0) {
	        ELOG_DEBUG("error sending the frame to the encoder");
	        return;
	    }

	    while (ret >= 0) {
	        ret = avcodec_receive_packet(aEncoderContext_, pkt);
	        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
	            return;
	        else if (ret < 0) {
	            ELOG_DEBUG("error encoding audio frame\n");
	            return;
	        }
	        
	        if (sink_ != NULL) {
	            sink_->receivePacket(*pkt, AVMEDIA_TYPE_AUDIO);
	        }
	        av_packet_unref(pkt);
	    }
    }
}



}	// Namespace payloader