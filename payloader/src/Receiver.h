#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Interfaces.h"

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "logger.h"
#include <queue>

namespace payloader {

class Receiver {
    DECLARE_LOGGER();
	public:
	    Receiver(const int mediaPort);
	    virtual ~Receiver();
	    int init();
	    void setSink(RtpReceiver* receiver);

	private:
		static const int LENGTH = 1500;
		boost::scoped_ptr<boost::asio::ip::udp::socket> socket_;
		boost::scoped_ptr<boost::asio::ip::udp::resolver> resolver_;
		boost::asio::io_service io_service_;
		boost::asio::ip::udp::endpoint sender_endpoint;
		char* buffer_[LENGTH];
		void handleReceive(const::boost::system::error_code& error, size_t bytes_recvd);
		RtpReceiver* sink_;
		
};
}	// Namespace payloader
#endif // RECEIVER_H_