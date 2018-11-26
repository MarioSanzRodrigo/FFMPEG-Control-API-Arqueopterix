#include "Streamer.h"


using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace payloader {

struct param{
  long int puerto;
};

DEFINE_LOGGER(Streamer, "Streamer");
Streamer::Streamer() {
    mp4Info.enabled = true;
    mp4Info.codec = AV_CODEC_ID_MPEG4;
    mp4Info.width = 704;
    mp4Info.height = 396;
    mp4Info.bitRate = 48000;
    mjpegInfo.enabled = true;
    mjpegInfo.codec = AV_CODEC_ID_MJPEG;
    mjpegInfo.width = 640;
    mjpegInfo.height = 480;
    mjpegInfo.bitRate = 4800;
    rv32Info.enabled = true;
    rv32Info.codec = AV_CODEC_ID_RAWVIDEO;
    rv32Info.width = 704;
    rv32Info.height = 396;
    rv32Info.bitRate = 4800;
    rv32Info.pix_fmt = AV_PIX_FMT_YUV420P;
    lheInfo.enabled = true;
    lheInfo.codec = AV_CODEC_ID_MLHE;
    lheInfo.width = 1680;
    lheInfo.height = 1050;
    lheInfo.bitRate = 48000;
}
Streamer::~Streamer(){
   
}

void Streamer::InitTransport(u_short aRtpPort, u_short aRtcpPort, bool TCP)
{
    sockaddr_in Server;  

    m_RtpClientPort  = aRtpPort;
    m_RtcpClientPort = aRtcpPort;
    m_TCPTransport   = TCP;

    if (!m_TCPTransport)
    {   // allocate port pairs for RTP/RTCP ports in UDP transport mode
        Server.sin_family      = AF_INET;   
        Server.sin_addr.s_addr = INADDR_ANY;   
        for (u_short P = 6970; P < 0xFFFE ; P += 2)
        {
            m_RtpSocket     = socket(AF_INET, SOCK_DGRAM, 0);     //creo el socket                
            Server.sin_port = htons(P);//asigno 6970 a la estrcutura
            if (bind(m_RtpSocket,(sockaddr*)&Server,sizeof(Server)) == 0)//asigno el puerto 6970 al socket para rtp
            {   // Rtp socket was bound successfully. Lets try to bind the consecutive Rtsp socket
                m_RtcpSocket = socket(AF_INET, SOCK_DGRAM, 0);
                Server.sin_port = htons(P + 1);//asigno 6971 a la estrcutura
                if (bind(m_RtcpSocket,(sockaddr*)&Server,sizeof(Server)) == 0) //asigno el puerto 6970 al socket para rtcp
                {
                    m_RtpServerPort  = P;
                    m_RtcpServerPort = P+1;
                    break; 
                }
                else
                {
                    close(m_RtpSocket);
                    close(m_RtcpSocket);
                };
            }
            else close(m_RtpSocket);
        };
    };
};

u_short Streamer::GetRtpServerPort()
{
    return m_RtpServerPort;
};

u_short Streamer::GetRtcpServerPort()
{
    return m_RtcpServerPort;
};
    
void Streamer::StartStreaming(){

    // camera: /dev/video0 vfwcap
   
    socklen_t RecvLen = sizeof(RecvAddr);
    // get client address for UDP transport
    getpeername(m_Client,(struct sockaddr*)&RecvAddr,&RecvLen);
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port   = htons(m_RtpClientPort);

    printf("RecvAddr.sin_port: %d\n", RecvAddr.sin_port);
    printf("RecvAddr.sin_family: %d\n", RecvAddr.sin_family);

    //payloader::InputReader* reader = new payloader::InputReader("/home/nacho/Documentos/Proyectos/Arqueopterix/pay_good/payloader/video_2.avi", NULL);
    payloader::InputReader* reader = new payloader::InputReader("/dev/video0", "vfwcap");
    payloader::Packager* packager = new payloader::Packager();
    payloader::Sender* sender = new payloader::Sender(RecvAddr, m_RtpSocket);
    
    // common
    packager->init();
    sender->init();

    // 4a sin transcodificación
    reader->setSink(packager);
       

    // 4b con transcodificación
    // encoder->init({}, lheInfo);
    // decoder->init({}, mp4Info);
    // reader->setSink(decoder);
    // decoder->setSink(encoder);
    // encoder->setSink(packager);

    // 4c desde webcam MJPEG
    // encoder->init({}, lheInfo);
    // decoder->init({}, mjpegInfo);
    // reader->setSink(decoder);
    // decoder->setSink(encoder);
    // encoder->setSink(packager);

    // 4e desde desktop
    // encoder->init({}, lheInfo);
    // decoder->init({}, rv32Info);
    // reader->setSink(decoder);
    // decoder->setSink(encoder);
    // encoder->setSink(packager);


    //Rtsp: InputReader -> sender_rtsp
    

    // common
    packager->setSink(sender);
    reader->init();

  return;
}

}




