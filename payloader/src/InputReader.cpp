#include "InputReader.h"
#include <sys/time.h>

namespace payloader {

DEFINE_LOGGER(InputReader, "InputReader");

InputReader::InputReader(const std::string& url, const char *device) : input_url_(url), input_device_(device) {
    ELOG_DEBUG("Creating source reader to %s", url.c_str());
    av_context_ = NULL;
    reading_ = false;
    sink_ = NULL;
}

InputReader::~InputReader() {
	// deliver_thread_.join();
	avformat_close_input(&av_context_);
}

int InputReader::init(){

	av_register_all();
	avdevice_register_all();
	av_context_ = avformat_alloc_context();

	AVInputFormat *a = NULL;
	if (input_device_ != NULL) {
		a = av_find_input_format(input_device_);
	}

	char errbuff[500];

	ELOG_DEBUG("Opening source %s", input_url_.c_str());
	int res = avformat_open_input(&av_context_, input_url_.c_str(), a, NULL);
	ELOG_DEBUG("Opening source result %d", res);
	if(res != 0){
		av_strerror(res, (char*)(&errbuff), 500);
		ELOG_ERROR("Error opening source %s", errbuff);
		return res;
    }

    ELOG_DEBUG("Finding stream info");
    res = avformat_find_stream_info(av_context_,NULL);
    ELOG_DEBUG("Finding stream info result %d", res);
    if(res < 0){
      av_strerror(res, (char*)(&errbuff), 500);
      ELOG_ERROR("Error finding stream info %s", errbuff);
      return res;
    }

    audio_stream_index_ = av_find_best_stream(av_context_, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  	if (audio_stream_index_ < 0)
    	ELOG_WARN("No Audio stream found");
    
    video_stream_index_ = av_find_best_stream(av_context_, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  	if (video_stream_index_ < 0)
    	ELOG_WARN("No Video stream found");

	int audio_codec = av_context_->streams[audio_stream_index_]->codecpar->codec_id;
	int video_codec = av_context_->streams[video_stream_index_]->codecpar->codec_id;
	ELOG_DEBUG("Audio codec %d, video codec %d", audio_codec, video_codec);

    ELOG_DEBUG("Video stream index %d, Audio Stream index %d", video_stream_index_, audio_stream_index_);

    //Crear SDP para TcpCpnnection
   /* char   SDPBuf[1024];
    av_sdp_create ( &av_context_,1,SDPBuf,1024) ;
    printf("SDP: %s\n",SDPBuf );*/

    this->startReading();

    return true;

}

void InputReader::setSink(PacketReceiver* receiver) {
	sink_ = receiver;
}


void InputReader::startReading() {

	AVPacket avpacket_;
	avpacket_.data = NULL;

	reading_ = true;

	while (av_read_frame(av_context_, &avpacket_) >= 0) {

		// ELOG_DEBUG("Readed packet pts: %ld, dts: %ld,  index %d", avpacket_.pts, avpacket_.dts, avpacket_.stream_index);
		
		if (sink_ != NULL) {

			AVMediaType type = AVMEDIA_TYPE_UNKNOWN;
			if (avpacket_.stream_index == video_stream_index_)
				type = AVMEDIA_TYPE_VIDEO;
			else if (avpacket_.stream_index == audio_stream_index_)
				type = AVMEDIA_TYPE_AUDIO;

			sink_->receivePacket(avpacket_, type);

		}
	}

	ELOG_DEBUG("Ended source reading");
	reading_ = false;
	av_packet_unref(&avpacket_);
}

void InputReader::deliverLoop() {
	
}

}	// Namespace payloader