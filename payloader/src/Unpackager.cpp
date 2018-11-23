#include "Unpackager.h"
#include <sys/time.h>
#include "rtp/RtpHeaders.h"

namespace payloader {

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