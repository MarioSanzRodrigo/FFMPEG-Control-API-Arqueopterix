#ifndef RTSPREADER_H
#define RTSPREADER_H

#include "Interfaces.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "logger.h"
#include <queue>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/avconfig.h>
	#include <libavdevice/avdevice.h>
}

 
namespace payloader {

#define DELIVERING_INTERVAL 1000

class RtspReader {
    DECLARE_LOGGER();
	public:
	    RtspReader(const std::string& url, const char *device);
	    virtual ~RtspReader();
	    int init();
	    void setSink(PacketReceiver* receiver);
	    void socketWriter();

	private:
	    AVFormatContext* av_context_;
	   	AVCodecContext *pCodecCtx;
	    std::string input_url_;
	    const char *input_device_;
	    std::queue<AVPacket> packet_queue_;
	    bool reading_;
	    PacketReceiver* sink_;
	    int video_stream_index_;
	    int audio_stream_index_;


	    boost::mutex queue_mutex_;
		boost::thread deliver_thread_;

	    void startReading();
	    void deliverLoop();
};
}	// Namespace payloader
#endif // RTSPREADER_H