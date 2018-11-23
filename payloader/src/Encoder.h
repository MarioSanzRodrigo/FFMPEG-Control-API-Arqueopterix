#ifndef ENCODER_H_
#define ENCODER_H_

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


class Encoder : public FrameReceiver {
    DECLARE_LOGGER();
	public:
	    Encoder();
	    virtual ~Encoder();
	    int init(AudioCodecInfo audioInfo, VideoCodecInfo videoInfo);
	    void setSink(PacketReceiver* receiver);
	    void receiveFrame(AVFrame* frame, AVMediaType type);

	private:
	   	PacketReceiver* sink_;
	   	AVCodec* vEncoder_;
	   	AVCodecContext* vEncoderContext_;
	   	AVCodec* aEncoder_;
	   	AVCodecContext* aEncoderContext_;

};
}	// Namespace payloader
#endif // ENCODER_H_