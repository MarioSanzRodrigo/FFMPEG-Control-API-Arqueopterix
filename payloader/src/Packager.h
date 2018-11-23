#ifndef PACKAGER_H_
#define PACKAGER_H_

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

#define PACKAGED_BUFFER_SIZE 2000

class Packager : public PacketReceiver {
    DECLARE_LOGGER();
	public:
	    Packager();
	    virtual ~Packager();
	    int init();
	    void setSink(RtpReceiver* receiver);
	    void receivePacket(AVPacket& packet, AVMediaType type);
	  
	 int init(AVCodecContext *pCodecCtx);
	 void setSink(FrameReceiver *receiver);

	 void sendPacket(AVPacket *pkt);

	private:
		RtpReceiver* sink_;
		int videoSeqNum_;
		unsigned char* outBuff_;
		unsigned char* rtpBuffer_;
};
}	// Namespace payloader
#endif // PACKAGER_H_