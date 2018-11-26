#ifndef SENDER_H_
#define SENDER_H_

#include "Interfaces.h"

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include "logger.h"
#include <queue>

namespace payloader {

class Sender : public RtpReceiver {
    DECLARE_LOGGER();
	public:
	    Sender( sockaddr_in RecvAddr, int m_RtpSocket);
	    virtual ~Sender();
	    int init();
		int init(const std::string& url, const std::string& port);

	    void receiveRtpPacket(unsigned char* inBuff, int buffSize);
	 	void sendPacket(AVPacket pkt, int video_stream_index_,  AVFormatContext *ifmt_ctx, AVFormatContext *ofmt_ctx, int64_t start_time, AVMediaType type);
		int sendData(char* buffer, int len);

	private:
		struct dataPacket{
		    char data[1500];
		    int length;
		};

		boost::scoped_ptr<boost::asio::ip::udp::socket> socket_;
		boost::scoped_ptr<boost::asio::ip::udp::resolver> resolver_;

		boost::scoped_ptr<boost::asio::ip::udp::resolver::query> query_;
		boost::asio::ip::udp::resolver::iterator iterator_;
		boost::asio::io_service io_service_;

		boost::thread send_Thread_;
		boost::condition_variable cond_;
		boost::mutex queueMutex_;
		std::queue<dataPacket> sendQueue_;
		bool sending_;

		int RtpSocket;
		sockaddr_in RecvAdd;

		void sendLoop();


};
}	// Namespace payloader
#endif // SENDER_H_