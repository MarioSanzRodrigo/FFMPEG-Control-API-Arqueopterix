#include "RtspReader.h"
#include <sys/time.h>
#include <iostream>

#include <boost/asio.hpp>



/*Clase encargada de la lectura via rtsp*/
namespace payloader {

DEFINE_LOGGER(RtspReader, "RtspReader");
using boost::asio::ip::tcp;

RtspReader::RtspReader(const std::string& url, const char *device) : input_url_(url), input_device_(device) {
    ELOG_DEBUG("Creating source reader to %s", url.c_str());
    av_context_ = NULL;
    reading_ = false;
    sink_ = NULL;
}

RtspReader::~RtspReader() {
	avformat_close_input(&av_context_);
}

int RtspReader::init(){
	avformat_network_init();
	av_register_all();
	avdevice_register_all();
	av_context_ = avformat_alloc_context();

	AVInputFormat *a = NULL;
	if (input_device_ != NULL) {
		a = av_find_input_format(input_device_);
	}

	//Probe connection
  //this->socketWriter();

   AVDictionary *opts = 0;
   av_dict_set(&opts, "rtsp_transport", "udp", 0);

	char errbuff[500];
  //av_context_->max_analyze_duration = 0;

	ELOG_DEBUG("Opening source %s", input_url_.c_str());
	int res = avformat_open_input(&av_context_, input_url_.c_str(), a, NULL);
	ELOG_DEBUG("Opening source result %d", res);
	if(res != 0){
		av_strerror(res, (char*)(&errbuff), 500);
		ELOG_ERROR("Error opening source: %s", errbuff);
		return res;
    }
    av_dump_format(av_context_,0,input_url_.c_str(),false);
    printf("There are:  %d\n", av_context_->nb_streams);
    unsigned int video_codec_id = -1;
           for (unsigned int i = 0; i < av_context_->nb_streams; ++i) {
              
               if(av_context_->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
               {
                   printf("Found video %d\n", i);
                   //Comprobar librerias al dia
                   /*printf("LIBAVFORMAT_VERSION_INT %d\n", LIBAVFORMAT_VERSION_INT);
                   printf("avformat_version %d\n", avformat_version());*/
                   video_codec_id = i;
               }
               
           }

   /* ELOG_DEBUG("Finding stream info");
    res = avformat_find_stream_info(av_context_,NULL);
    printf("RES %d\n", res);
    ELOG_DEBUG("Finding stream info result %d", res);
    if(res < 0){
      av_strerror(res, (char*)(&errbuff), 500);
      ELOG_ERROR("Error finding stream info %s", errbuff);
      return res;
    }*/

    audio_stream_index_ = av_find_best_stream(av_context_, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  	if (audio_stream_index_ < 0)
    	ELOG_WARN("No Audio stream found");
    
    video_stream_index_ = av_find_best_stream(av_context_, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  	if (video_stream_index_ < 0){
    	ELOG_WARN("No Video stream found");
      return false;
    }

    // Get a pointer to the codec context for the video stream
    pCodecCtx=av_context_->streams[video_stream_index_]->codec;

    //int audio_codec = av_context_->streams[audio_stream_index_]->codecpar->codec_id;
    int video_codec = av_context_->streams[video_stream_index_]->codecpar->codec_id;
    //ELOG_DEBUG("Audio codec %d, video codec %d", audio_codec, video_codec);
    ELOG_DEBUG("Video codec id: %d", video_codec);
    ELOG_DEBUG("Video stream index %d, Audio Stream index %d", video_stream_index_, audio_stream_index_);
    
    this->startReading();

    return true;
}

void RtspReader::setSink(PacketReceiver* receiver) {
	sink_ = receiver;
}

void RtspReader::socketWriter() {
	ELOG_DEBUG("pidiendo");
	 try
  {
    // Any program that uses asio need to have at least one io_service object
    boost::asio::io_service io_service;

    // Convert the server name that was specified as a parameter to the application, to a TCP endpoint. 
    // To do this, we use an ip::tcp::resolver object.
    tcp::resolver resolver(io_service);

    // A resolver takes a query object and turns it into a list of endpoints. 
    // We construct a query using the name of the server, specified in argv[1], 
    // and the name of the service, in this case "daytime == 13".
    printf("peticion hecha\n");
    tcp::resolver::query query("138.4.7.72", "8854");

    // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator. 
    // A default constructed ip::tcp::resolver::iterator object can be used as an end iterator.
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Now we create and connect the socket.
    // The list of endpoints obtained above may contain both IPv4 and IPv6 endpoints, 
    // so we need to try each of them until we find one that works. 
    // This keeps the client program independent of a specific IP version. 
    // The boost::asio::connect() function does this for us automatically.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // The connection is open. All we need to do now is read the response from the daytime service.
    for (;;)
    {
      // We use a boost::array to hold the received data. 
      boost::array<char, 128> buf;
      boost::system::error_code error;

      // The boost::asio::buffer() function automatically determines 
      // the size of the array to help prevent buffer overruns.
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      // When the server closes the connection, 
      // the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof error, 
      // which is how we know to exit the loop.
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
      std::cout.write(buf.data(), len);
    }
  }
  // handle any exceptions that may have been thrown.
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

}
void RtspReader::startReading() {
	
	AVPacket avpacket_;
	avpacket_.data = NULL;
	//Aquí tenemos que conseguir pasarle al decoder este pCodecCtx
	sink_->init(pCodecCtx);

	reading_ = true;
  printf("Esperando frames...\n");
	while (av_read_frame(av_context_, &avpacket_) >= 0) {
 
		ELOG_DEBUG("Readed packet pts: %ld, dts: %ld,  index %d", avpacket_.pts, avpacket_.dts, avpacket_.stream_index);
		
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

void RtspReader::deliverLoop() {
}

}	// Namespace payloader