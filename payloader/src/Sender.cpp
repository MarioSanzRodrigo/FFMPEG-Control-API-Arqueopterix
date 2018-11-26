#include "Sender.h"
#include <sys/time.h>


namespace payloader {

DEFINE_LOGGER(Sender, "Sender");

Sender::Sender( sockaddr_in RecvAddr, int m_RtpSocket) : RtpSocket(m_RtpSocket), RecvAdd(RecvAddr) {
    printf("Iniciando sender\n");
}

Sender::~Sender() {
    sending_ = false;
    send_Thread_.join();
}
int Sender::init(const std::string& url, const std::string& port){
    return 0;
}

int Sender::init() {
    sending_ = true;
    send_Thread_ = boost::thread(&Sender::sendLoop, this);
    return true;
}
void Sender::sendPacket(AVPacket pkt, int video_stream_index_,  AVFormatContext *ifmt_ctx, AVFormatContext *ofmt_ctx, int64_t start_time, AVMediaType type){
}
void Sender::receiveRtpPacket(unsigned char* inBuff, int buffSize) {
    boost::mutex::scoped_lock lock(queueMutex_);
    if (sending_ == false)
        return;
    if (sendQueue_.size() < 1000) {
        dataPacket p_;
        memcpy(p_.data, inBuff, buffSize);
        p_.length = buffSize;
        sendQueue_.push(p_);
    }
    cond_.notify_one();
}

void Sender::sendLoop(){
    while (sending_ ) {
        boost::unique_lock<boost::mutex> lock(queueMutex_);
        while (sendQueue_.size() == 0) {
            cond_.wait(lock);
            if (!sending_) {
                return;
            }
        }
        this->sendData(sendQueue_.front().data, sendQueue_.front().length);
        sendQueue_.pop();
    }
}

int Sender::sendData(char* buffer, int len) {
    ELOG_DEBUG("Sending socket %d", len);
    boost::asio::mutable_buffer b1 = boost::asio::buffer(buffer, len);
    std::size_t s1 = boost::asio::buffer_size(b1);
    unsigned char* RtpBuf2 = boost::asio::buffer_cast<unsigned char*>(b1);
    sendto(RtpSocket,RtpBuf2,len,0,(sockaddr *) & RecvAdd,sizeof(RecvAdd));// mando el paquete por el socket rtp a la recvaddr
    return len;
}

}	// Namespace payloader