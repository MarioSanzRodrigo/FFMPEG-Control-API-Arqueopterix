#ifndef DECODER_H_
#define DECODER_H_

#include "Interfaces.h"
#include "Codecs.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "logger.h"
#include <queue>

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/avconfig.h>
}


namespace payloader {

class Decoder : public PacketReceiver {
    DECLARE_LOGGER();
	public:
	    Decoder();
	    virtual ~Decoder();
	    int init(AVCodecContext *pCodecCtx);
	    void setSink(FrameReceiver* receiver);
	    void setSink(RtpReceiver* receiver);
	    void receivePacket(AVPacket& packet, AVMediaType type);
		void sendPacket(AVPacket *pkt);


	private:
	   	FrameReceiver* sink_;
	   //	FrameReceiver* sink_1;
	   	AVCodec* vDecoder_;
	   	AVCodecContext* vDecoderContext_;
	   	AVCodecContext* vDecoderContext_orig;
	   	AVFrame* vFrame_;
	   	AVCodec* aDecoder_;
	   	AVCodecContext* aDecoderContext_;
	   	AVFrame* aFrame_;
};
}	// Namespace payloader
#endif // DECODER_H_