/*################################################
######### INTEGRACION PAYLOADER UPM ##############
#################################################*/

#ifndef UNPACKAGER_H_
#define UNPACKAGER_H_

#include "Interfaces.h"
#include "Codecs.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "logger.h"
#include <queue>

#include "Unpackager.h"
#include <sys/time.h>
#include "rtp/RtpHeaders.h"

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/avconfig.h>
}

namespace payloader {

#define UNPACKAGED_BUFFER_SIZE 1500000

class Unpackager : public RtpReceiver {
    DECLARE_LOGGER();
	public:
	    Unpackager();
	    virtual ~Unpackager();
	    int init();
	    void setSink(PacketReceiver* receiver);
	    void receiveRtpPacket(unsigned char* inBuff, int buffSize);

	private:
		PacketReceiver* sink_;
		int upackagedSize_;
		unsigned char* unpackagedBuffer_;
		long long first_video_timestamp_;
};
#endif // UNPACKAGER_H_

DEFINE_LOGGER(Unpackager, "Unpackager");

Unpackager::Unpackager() {
    sink_ = NULL;
    unpackagedBuffer_ = NULL;
    upackagedSize_ = 0;
}

Unpackager::~Unpackager() {
    free(unpackagedBuffer_); 
    unpackagedBuffer_ = NULL;
}

int Unpackager::init() {

    unpackagedBuffer_ = (unsigned char*) malloc(UNPACKAGED_BUFFER_SIZE);
    first_video_timestamp_ = -1;

    return true;
}

void Unpackager::setSink(PacketReceiver* receiver) {
	sink_ = receiver;
}

void Unpackager::receiveRtpPacket(unsigned char* inBuff, int buffSize) {

    RtpHeader* head = reinterpret_cast<RtpHeader*>(inBuff);
    

    // ELOG_DEBUG("Received RTP packet size %d timestamp %d", buffSize, head->timestamp);

    AVMediaType type = AVMEDIA_TYPE_UNKNOWN;


    // if (avpacket_.stream_index == video_stream_index_)
        type = AVMEDIA_TYPE_VIDEO;
    // else if (avpacket_.stream_index == audio_stream_index_)
     // type = AVMEDIA_TYPE_AUDIO;


    // int pt = head->getPayloadType();

    int inBuffOffset = head->getHeaderLength();
    int l = buffSize - head->getHeaderLength();
    
    memcpy(unpackagedBuffer_, &inBuff[inBuffOffset], l);

    upackagedSize_ += l;
    unpackagedBuffer_ += l;

    if (head->getMarker()) {
        ELOG_DEBUG("Tengo un frame desempaquetado!! Size = %d", upackagedSize_);

        unpackagedBuffer_ -= upackagedSize_;

        AVPacket pkt;
        av_init_packet(&pkt);
        
        pkt.data = unpackagedBuffer_;
        pkt.size = upackagedSize_;

        if (first_video_timestamp_ == -1) {
            first_video_timestamp_ = head->getTimestamp();
        }

        // working on this

        ELOG_DEBUG("Veamos %lld, %lld", head->getTimestamp(), first_video_timestamp_);


        long long timestampToWrite = (head->getTimestamp() - first_video_timestamp_) / (90000 / 1000);

        // pkt.pts = timestampToWrite;
        pkt.dts = timestampToWrite;
        ELOG_DEBUG("Voy a escribir paquetr %d", pkt.dts);

        if (sink_ != NULL) {

            sink_->receivePacket(pkt, type);

        }

        upackagedSize_ = 0;

    }

    // if (type == AVMEDIA_TYPE_VIDEO) {

        
    
    // } else if (type == AVMEDIA_TYPE_AUDIO) {
    


    // }

}


}	// Namespace payloader

