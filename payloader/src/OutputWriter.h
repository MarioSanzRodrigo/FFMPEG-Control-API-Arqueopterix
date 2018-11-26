#ifndef OUTPUTWRITER_H_
#define OUTPUTWRITER_H_

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
	#include <libavutil/time.h>
	#include <libswscale/swscale.h>
}

#include <stdio.h>
#include <opencv2/opencv.hpp>

namespace payloader {


class OutputWriter : public PacketReceiver, public FrameReceiver {
    DECLARE_LOGGER();
	public:
	    OutputWriter(const std::string& url);
	    virtual ~OutputWriter();
	    int init(AudioCodecInfo audioInfo, VideoCodecInfo videoInfo);
	    int init(AVCodecContext *pCodecCtx);
	    void receivePacket(AVPacket& packet, AVMediaType type);
	    void receiveFrame(AVFrame* frame, AVMediaType type);
	    void setSink(FrameReceiver* receiver);
		void setSink(RtpReceiver *receiver);
	    void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);
	    void sendPacket(AVPacket *pkt);

		//void setSink(PacketReceiver* receiver);

	private:
		AVFormatContext* av_context_;
		std::string output_url_;
		AVStream *video_stream_;
		AVStream *audio_stream_;
		FrameReceiver* sink_;
		int i,one;
		cv::VideoWriter output;
};
}	// Namespace payloader
#endif // OUTPUTWRITER_H_
