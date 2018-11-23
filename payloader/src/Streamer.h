#ifndef _Streamer_H
#define _Streamer_H

#include <sys/types.h>
#include <sys/socket.h>
#include "Codecs.h"
#include "logger.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include  <pthread.h>
#include  <stdlib.h>
#include  <unistd.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp> 
#include <unistd.h>



#include "Decoder.h"
#include "Encoder.h"
#include "Receiver.h"
#include "Sender.h"
#include "Codecs.h"
#include "InputReader.h"
#include "Packager.h"

namespace payloader {
class Streamer{
    DECLARE_LOGGER();
    public:
        Streamer();
         virtual ~Streamer();

        void    InitTransport(u_short aRtpPort, u_short aRtcpPort, bool TCP);
        u_short GetRtpServerPort();
        u_short GetRtcpServerPort();
        void    StartStreaming();

    private:

        int  m_RtpSocket;          // RTP socket for streaming RTP packets to client
        int  m_RtcpSocket;         // RTCP socket for sending/receiving RTCP packages

        u_short m_RtpClientPort;      // RTP receiver port on client 
        u_short m_RtcpClientPort;     // RTCP receiver port on client
        u_short m_RtpServerPort;      // RTP sender port on server 
        u_short m_RtcpServerPort;     // RTCP sender port on server

        u_short m_SequenceNumber;
        long int   m_Timestamp;
        int     m_SendIdx;
        bool    m_TCPTransport;
        int  m_Client;

        sockaddr_in RecvAddr;

        payloader::VideoCodecInfo mp4Info;
        payloader::VideoCodecInfo mjpegInfo;
        payloader::VideoCodecInfo rv32Info;
        payloader::VideoCodecInfo lheInfo;
        
};
}

#endif