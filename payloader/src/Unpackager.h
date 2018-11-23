#ifndef UNPACKAGER_H_
#define UNPACKAGER_H_

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
}	// Namespace payloader
#endif // UNPACKAGER_H_