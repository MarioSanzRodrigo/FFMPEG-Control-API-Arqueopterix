#include "Receiver.h"
#include <sys/time.h>
using boost::asio::ip::udp;

namespace payloader {

DEFINE_LOGGER(Receiver, "Receiver");

Receiver::Receiver(const int mediaPort) {
    socket_.reset(new udp::socket(io_service_, udp::endpoint(udp::v4(), mediaPort)));
    resolver_.reset(new udp::resolver(io_service_));
    sink_ = NULL;
}

Receiver::~Receiver() {
    io_service_.stop();
}

int Receiver::init() {
    socket_->async_receive_from(boost::asio::buffer(buffer_, LENGTH), sender_endpoint, 
        boost::bind(&Receiver::handleReceive, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    
    io_service_.run();

    return true;
}

void Receiver::setSink(RtpReceiver* receiver) {
    sink_ = receiver;
}

void Receiver::handleReceive(const::boost::system::error_code& error, size_t bytes_recvd) {

    if (error) {
      ELOG_DEBUG("ERROR ");
    }
    if (bytes_recvd > 0 && this->sink_){
      ELOG_DEBUG("Received data %d", (int)bytes_recvd);
      this->sink_->receiveRtpPacket((unsigned char*)buffer_, (int)bytes_recvd);
      socket_->async_receive_from(boost::asio::buffer(buffer_, LENGTH), sender_endpoint, 
        boost::bind(&Receiver::handleReceive, this, boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
}

}	// Namespace payloader