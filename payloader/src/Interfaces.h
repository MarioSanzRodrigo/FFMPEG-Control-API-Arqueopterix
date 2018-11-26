#ifndef INTERFACES_H_
#define INTERFACES_H_
#include <string>
extern "C" {
	#include <libavformat/avformat.h>
}

namespace payloader {

class PortReceiver{  
    public:  
        virtual void tomaPuerto(long int puerto) = 0;  
        virtual ~PortReceiver(){};  
};  

class FrameReceiver {
public:
	virtual void receiveFrame(AVFrame* frame, AVMediaType type) = 0;
	virtual ~FrameReceiver() {}
};


class RtpReceiver {
public:
	virtual void receiveRtpPacket(unsigned char* inBuff, int buffSize) = 0;
	virtual int init() = 0;
	virtual int init(const std::string& url, const std::string& port) = 0;
	//Aunque pasemos en la invoacion por valor, si esta declarado como puntero se pasa como puntero
	virtual void sendPacket(AVPacket pkt, int video_stream_index_,  AVFormatContext *ifmt_ctx, AVFormatContext *ofmt_ctx, int64_t start_time, AVMediaType type) = 0;
	virtual int sendData(char* buffer, int len) = 0;
	virtual ~RtpReceiver() {}
};


class PacketReceiver {
public:
	virtual void receivePacket(AVPacket& packet, AVMediaType type) = 0;
	virtual int init(AVCodecContext *pCodecCtx) = 0;
	virtual void setSink(FrameReceiver *receiver) = 0;
	virtual void setSink(RtpReceiver *receiver) = 0;
	virtual void sendPacket(AVPacket *pkt) = 0;
	//virtual void setSink(PacketReceiver* receiver) = 0;
	virtual ~PacketReceiver() {}
};

}	// Namespace payloader

#endif // INTERFACES_H_ 