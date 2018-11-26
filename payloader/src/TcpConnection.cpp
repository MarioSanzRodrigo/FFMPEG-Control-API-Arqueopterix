#include "TcpConnection.h"

std::string make_daytime_string(){
  std::time_t now = std::time(0);
  return std::ctime(&now);
}
 pthread_mutex_t mutex;
// Using shared_ptr and enable_shared_from_this 
// because we want to keep the tcp_connection object alive 
// as long as there is an operation that refers to it.
namespace payloader { 
DEFINE_LOGGER(TcpConnection, "TcpConnection");
// Constructor: initialises an acceptor to listen on TCP port 8554.
TcpConnection::TcpConnection(int Client, payloader::Streamer* streamer): m_Streamer(streamer), rtspClientPort(Client){//Creamos el soocket en el puerto 8554 escuchando tcp para la conexión rtsp.

    m_RtspSessionID  = rand() << 16;         // create a session ID
    m_RtspSessionID |= rand();
    m_RtspSessionID |= 0x80000000;         
    m_StreamID       = -1;
    m_ClientRTPPort  =  0;
    m_ClientRTCPPort =  0;
    m_RtpServerPort =   0;
    m_RtcpServerPort  = 0;
    m_TcpTransport   =  false;


    m_RtspCmdType   = RTSP_UNKNOWN;
    memset(m_URLPreSuffix, 0x00, sizeof(m_URLPreSuffix));
    memset(m_URLSuffix,    0x00, sizeof(m_URLSuffix));
    memset(m_CSeq,         0x00, sizeof(m_CSeq));
    memset(m_URLHostPort,  0x00, sizeof(m_URLHostPort));
    m_ContentLength  =  0;

}
/*
TcpConnection::TcpConnection(boost::asio::io_service &io_service, payloader::Streamer m_Streamer): socket_(io_service), m_Streamer(m_Streamer){
}*/
TcpConnection::~TcpConnection(){  
}

long int TcpConnection::getPort(){
  return puerto;
}

void TcpConnection::registerParent(PortReceiver *parent){
  this->myparent = parent;
}
void TcpConnection::DateHeader() {
    time_t tt = time(NULL);
    strftime(buf_Date, sizeof buf_Date, "Date: %a, %b %d %Y %H:%M:%S GMT", gmtime(&tt));
}
pthread_mutex_t TcpConnection::getMutex() {
    return mutex;
}
/*typedef boost::shared_ptr<TcpConnection> pointer;
boost::shared_ptr<TcpConnection> TcpConnection::create(boost::asio::io_service& io_service, payloader::Streamer *m_Streamer){
  return boost::shared_ptr<TcpConnection>(new TcpConnection(io_service,m_Streamer));
}*/
payloader::Streamer* TcpConnection::getStreamer(){
    return m_Streamer;
}

RTSP_CMD_TYPES TcpConnection::getC(){
    return C;
}

RTSP_CMD_TYPES TcpConnection::Handle_RtspRequest(char const * aRequest, unsigned aRequestSize){
  if(ParseRtspRequest(aRequest,aRequestSize)){
    DateHeader();
      switch (m_RtspCmdType){
        case RTSP_OPTIONS:  { Handle_RtspOPTION();   break; };
        case RTSP_DESCRIBE: { Handle_RtspDESCRIBE(); break; };
        case RTSP_SETUP:    { Handle_RtspSETUP(); break; };
        case RTSP_PLAY:     { Handle_RtspPLAY();     break; };
        case RTSP_PAUSE:     { Handle_RtspPAUSE();     break; };
        default: {};
    };
  };
return m_RtspCmdType;
}

void TcpConnection::Handle_RtspOPTION(){
    snprintf(response,sizeof(response),
    "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
    "Public: DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE\r\n\r\n",m_CSeq);
    std::string response1 = response;
    send(rtspClientPort,response,strlen(response),0);  
}
void TcpConnection::Handle_RtspPAUSE(){
    printf("EN PAUSE\n");
    char   response[1024];
    // simulate PAUSE server response
    snprintf(response,sizeof(response),
        "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
        "%s\r\n"
        "Range: npt=339.072020-\r\n"
        "Session: %i\r\n\r\n",
        m_CSeq,
        buf_Date,
        m_RtspSessionID);

    std::string response1 = response;
    send(rtspClientPort,response,strlen(response),0);   
}

void TcpConnection::Handle_RtspDESCRIBE(){
  char   response[1024];                     // SDP string size
  char   SDPBuf[1024];
  char   URLBuf[1024];

  // check whether we know a stream with the URL which is requested
  m_StreamID = -1;        // invalid URL
  if ((strcmp(m_URLPreSuffix,"ej") == 0) && (strcmp(m_URLSuffix,"1") == 0)) m_StreamID = 0; else
  if ((strcmp(m_URLPreSuffix,"ej") == 0) && (strcmp(m_URLSuffix,"2") == 0)) m_StreamID = 1;
  if (m_StreamID == -1){  
    // Stream not available
    snprintf(response,sizeof(response),
    "RTSP/1.0 404 Stream Not Found\r\nCSeq: %s\r\n%s\r\n",
    m_CSeq, 
    buf_Date);
    send(rtspClientPort,response,strlen(response),0);  
    return;
    };

    // simulate DESCRIBE server response
    char OBuf[256];
    char * ColonPtr;
    strcpy(OBuf,m_URLHostPort);
    ColonPtr = strstr(OBuf,":");
    if (ColonPtr != nullptr) ColonPtr[0] = 0x00;

   snprintf(SDPBuf,sizeof(SDPBuf),
        "v=0\r\n"
        "o=- 0 0 IN IP4 %s\r\n"           
        "s=Unnamed\r\n"   
        "t=0 0\r\n"                                            // start / stop - 0 -> unbounded and permanent session
        /*"c=IN IP4 0.0.0.0\r\n"*/
        "m=video 0 RTP/AVP 96\r\n"  
        "b=AS:1561\r\n"
        "a=rtpmap:96 MP4V-ES/90000\r\n"
        "a=fmtp:96 profile-level-id=1; config=000001B0F5000001B509000001000000012008BC040687FFFF0AAD8B0218CA31000001B25876694430303530\r\n"  
        "a=control:streamid=0\r\n",                     // currently we just handle UDP sessions
         OBuf);

/*
    snprintf(SDPBuf,sizeof(SDPBuf),
       "v=0\r\n"
        "o=- %d 1 IN IP4 %s\r\n"
        "s=Unnamed\r\n"
        "i=N/A\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "t=0 0\r\n"
        "a=tool:vlc 2.2.5.1 \r\n"
        "a=rcvonly\r\n"
        "a=type:broadcast\r\n"
        "a=charset:UTF-8\r\n"
        "a=control:rtsp://138.4.7.72:8554/ej/1\r\n"
        "m=audio 0 RTP/AVP 96\r\n"
        "b=AS:1536\r\n"
        "b=RR:0\r\n"
        "a=rtpmap:96 L16/4800/2\r\n"
        "a=control:rtsp://138.4.7.72:8554/ej/1/trackID=0\r\n"
        "m=video 0 RTP/AVP 96\r\n"
        "b=RR:0\r\n"
        "a=rtpmap:96 MP4V-ES/90000\r\n"
        "a=fmtp:96 profile-level-id=3; config=0000012008bc040687ffff0aad8b0218ca31;\r\n"
        "a=control:rtsp://138.4.7.72:8554/ej/1/trackID=1\r\n",
        rand(),
        OBuf);*/

    /*snprintf(SDPBuf,sizeof(SDPBuf),//AUDIO 1 VIDEO 2 -----
        "v=0\r\n"
        "o=- %d IN IP4 %s\r\n"
        "s=No name\r\n"
        "t=0 0\r\n"
        "a=tool:libavformat 57.72.101 \r\n"
        "m=audio 0 RTP/AVP 97\r\n"
        "b=AS:1536\r\n"
        "a=control:streamid=0\r\n"
        "m=video 0 RTP/AVP 96\r\n"
        "b=AS:1561\r\n"
        "a=rtpmap:96 MP4V-ES/90000\r\n"//"rtpmap" is used to specify what the media is
        "a=fmtp:97 profile-level-id=1; config=000001B0F5000001B509000001000000012008BC040687FFFF0AAD8B0218CA31000001B25876694430303530\r\n"
        "a=control:streamid=1\r\n",
        rand(),
        OBuf);*/


    //De prueba segura
    /* snprintf(SDPBuf,sizeof(SDPBuf), 
        "v=0\r\n"
        "o=- %d 1 IN IP4 %s\r\n"
        "s=RTP Session\r\n"
        "i=An Example of RTSP Session Usage\r\n"
        "e=adm@example.com"
        "c=IN IP4 0.0.0.0\r\n"
        "a=control: *\r\n"
        "a=range:npt=00:00:00-00:10:34.10"
        "t=0 0\r\n"
        "m=audio 0 RTP/AVP 0\r\n"
        "b=AS:1536\r\n"
        "b=RR:0\r\n"
        "a=rtpmap:96 L16/4800/2\r\n"
        "a=control:trackID=1\r\n"
        "m=video 0 RTP/AVP 26\r\n"
        "b=RR:0\r\n"
        "a=rtpmap:96 MP4V-ES/90000\r\n"
        "a=fmtp:96 profile-level-id=3; config=0000012008bc040687ffff0aad8b0218ca31;\r\n"
        "a=control:trackID=4\r\n",
        rand(),
        OBuf);*/
    //int ran = rand();
  // snprintf(SDPBuf,sizeof(SDPBuf),
       /* "v=0\r\n"
        "o=- %d 1 IN IP4 %s\r\n"           
        "s=\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "t=0 0\r\n"
        "a=tool:vlc 2.2.5.1 \r\n"
        "a=rcvonly\r\n"*/
        /*"a=type:broadcast\r\n"*/
        /*"a=charset:UTF-8\r\n"*/
/*        "a=control:rtsp://138.4.7.72:8554/ej/1/\r\n"    */                                        // start / stop - 0 -> unbounded and permanent session
        /*"m=video 0 RTP/AVP 96\r\n" */
        /*"b=RR:0\r\n"*/
        /*"a=rtpmap:96 MP4V-ES/90000\r\n"
        "a=fmtp:96 profile-level-id=3; config=0000012008bc040687ffff0aad8b0218ca31;\r\n"
        "a=control:trackID=0\r\n"
        "m=audio 0 RTP/AVP 96\r\n"*/
        /*"b=AS:1536\r\n"*/
        /*"b=RR:0\r\n"*/
       /* "a=rtpmap:96 L16/4800/2\r\n"
        "a=control:trackID=1\r\n",                        // currently we just handle UDP sessions
        rand(),
        OBuf);*/
 
    char StreamName[64];
    switch (m_StreamID)
    {
        case 0: strcpy(StreamName,"ej/1"); break;
        case 1: strcpy(StreamName,"ej/2"); break;
    };
    snprintf(URLBuf,sizeof(URLBuf),
        "rtsp://%s/%s",
        m_URLHostPort,
        StreamName);

    snprintf(response,sizeof(response),
        "RTSP/1.0 200 OK\r\n"
        //"Server: VLC/2.2.5.1\r\n"
        "%s\r\n"    //fecha
        "Content-Type: application/sdp\r\n"
        "Content-Base: %s/\r\n"   //URLBuf
        "Content-Length: %d\r\n"    //strlen(SDPBuf)
        //"Cache-Control: no-cache\r\n"
        "CSeq: %s\r\n\r\n"
        "%s",   //SDPBuf
        buf_Date,
        URLBuf,
        strlen(SDPBuf),
        m_CSeq,
        SDPBuf);
      
   std::string response1 = response;
   send(rtspClientPort,response,strlen(response),0);    
};
void TcpConnection::Handle_RtspSETUP(){
    char response[1024];
    char Transport[255];
    boost::array<char, 256> buff; 

    // init RTP streamer transport type (UDP or TCP) and ports for UDP transport
    m_Streamer->InitTransport(m_ClientRTPPort,m_ClientRTCPPort,m_TcpTransport);

    // simulate SETUP server response
    if (m_TcpTransport){
      printf("TCP streaming: %d\n", m_TcpTransport);
        snprintf(Transport,sizeof(Transport),"RTP/AVP/TCP;unicast;interleaved=0-1");//interleaved para mezclar RTP y RTCP
    }
    else{
          printf("TCP streaming: %d, UDP ready.\n", m_TcpTransport);
     snprintf(Transport,sizeof(Transport),
            "RTP/AVP/UDP;unicast;client_port=%i-%i;server_port=%i-%i;",
            m_ClientRTPPort,
            m_ClientRTCPPort,
            m_Streamer->GetRtpServerPort(),
            m_Streamer->GetRtcpServerPort()
            );
    snprintf(response,sizeof(response),
        "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
        "%s\r\n"
        "Transport: %s\r\n"
        "Session: %i;\r\n\r\n",
        m_CSeq,
        buf_Date,
        Transport,
        m_RtspSessionID);

    std::string response1 = response;
    send(rtspClientPort,response,strlen(response),0);  
  }
};
void TcpConnection::Handle_RtspPLAY(){
    char   response[1024];
    // simulate PLAY server response
    snprintf(response,sizeof(response),
        "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
        "%s\r\n"
        "RTP-Info: url=rtsp://138.4.7.72:8554/ej/1/video;seq=12312232;rtptime=650\r\n"
        "Range: npt=0.000-\r\n"
        "Session: %i\r\n\r\n",
        m_CSeq,
        buf_Date,
        m_RtspSessionID);

    connected = true;

    std::string response1 = response;
    send(rtspClientPort,response,strlen(response),0);  
};

bool TcpConnection::ParseRtspRequest(char const * aRequest, unsigned aRequestSize){
    char     CmdName[RTSP_PARAM_STRING_MAX];
    char     CurRequest[RTSP_BUFFER_SIZE];
    unsigned CurRequestSize; 

    
    CurRequestSize = aRequestSize;
    memcpy(CurRequest,aRequest,aRequestSize);

    // check whether the request contains information about the RTP/RTCP UDP client ports (SETUP command)
    char * ClientPortPtr;
    char * TmpPtr;
    char   CP[1024];
    char * pCP;
  
    ClientPortPtr = strstr(CurRequest,"client_port");
    if (ClientPortPtr != nullptr)
    {
        TmpPtr = strstr(ClientPortPtr,"\r\n");
        if (TmpPtr != nullptr) 
        {
            TmpPtr[0] = 0x00;
            strcpy(CP,ClientPortPtr);
            pCP = strstr(CP,"=");
            if (pCP != nullptr)
            {
                pCP++;
                strcpy(CP,pCP);
                pCP = strstr(CP,"-");
                if (pCP != nullptr)
                {
                    pCP[0] = 0x00;
                    m_ClientRTPPort  = atoi(CP);
                    m_ClientRTCPPort = m_ClientRTPPort + 1;
                    //puerto = (intptr_t)m_ClientRTPPort;
                    //this->myparent->tomaPuerto(puerto);
                };
            };
        };
    };

    // Read everything up to the first space as the command name
    bool parseSucceeded = false;
    unsigned i;
    for (i = 0; i < sizeof(CmdName)-1 && i < CurRequestSize; ++i) 
    {
        char c = CurRequest[i];
        if (c == ' ' || c == '\t') 
        {
            parseSucceeded = true;
            break;
        }
        CmdName[i] = c;
    }
    CmdName[i] = '\0';
    if (!parseSucceeded) return false;

    // find out the command type
    if (strstr(CmdName,"OPTIONS")   != nullptr){ m_RtspCmdType = RTSP_OPTIONS;}  else
    if (strstr(CmdName,"DESCRIBE")  != nullptr){ m_RtspCmdType = RTSP_DESCRIBE;} else
    if (strstr(CmdName,"SETUP")     != nullptr) m_RtspCmdType = RTSP_SETUP;    else
    if (strstr(CmdName,"PLAY")      != nullptr) m_RtspCmdType = RTSP_PLAY;     else
     if (strstr(CmdName,"PAUSE")      != nullptr) m_RtspCmdType = RTSP_PAUSE;     else
    if (strstr(CmdName,"TEARDOWN")  != nullptr) m_RtspCmdType = RTSP_TEARDOWN;
    
    // check whether the request contains transport information (UDP or TCP)
    if (m_RtspCmdType == RTSP_SETUP)
    {
        TmpPtr = strstr(CurRequest,"RTP/AVP/TCP");
        if (TmpPtr != nullptr) m_TcpTransport = true; else m_TcpTransport = false;
    };

    // Skip over the prefix of any "rtsp://" or "rtsp:/" URL that follows:
    unsigned j = i+1;
    while (j < CurRequestSize && (CurRequest[j] == ' ' || CurRequest[j] == '\t')) ++j; // skip over any additional white space
    for (; (int)j < (int)(CurRequestSize-8); ++j) 
    {
        if ((CurRequest[j]   == 'r' || CurRequest[j]   == 'R')   && 
            (CurRequest[j+1] == 't' || CurRequest[j+1] == 'T') && 
            (CurRequest[j+2] == 's' || CurRequest[j+2] == 'S') && 
            (CurRequest[j+3] == 'p' || CurRequest[j+3] == 'P') && 
             CurRequest[j+4] == ':' && CurRequest[j+5] == '/') 
        {
            j += 6;
            if (CurRequest[j] == '/') 
            {   // This is a "rtsp://" URL; skip over the host:port part that follows:
                ++j;
                unsigned uidx = 0;
                while (j < CurRequestSize && CurRequest[j] != '/' && CurRequest[j] != ' ') 
                {   // extract the host:port part of the URL here
                    m_URLHostPort[uidx] = CurRequest[j];
                    uidx++;
                    ++j;
                };
            } 
            else --j;
            i = j;
            break;
        }
    }

    // Look for the URL suffix (before the following "RTSP/"):
    parseSucceeded = false;
    for (unsigned k = i+1; (int)k < (int)(CurRequestSize-5); ++k) 
    {
        if (CurRequest[k]   == 'R'   && CurRequest[k+1] == 'T'   && 
            CurRequest[k+2] == 'S'   && CurRequest[k+3] == 'P'   && 
            CurRequest[k+4] == '/') 
        {
            while (--k >= i && CurRequest[k] == ' ') {}
            unsigned k1 = k;
            while (k1 > i && CurRequest[k1] != '/') --k1;
            if (k - k1 + 1 > sizeof(m_URLSuffix)) return false;
            unsigned n = 0, k2 = k1+1;

            while (k2 <= k) m_URLSuffix[n++] = CurRequest[k2++];
            m_URLSuffix[n] = '\0';

            if (k1 - i > sizeof(m_URLPreSuffix)) return false;
            n = 0; k2 = i + 1;
            while (k2 <= k1 - 1) m_URLPreSuffix[n++] = CurRequest[k2++];
            m_URLPreSuffix[n] = '\0';
            i = k + 7; 
            parseSucceeded = true;
            int i= 0;
            break;
        }
    }
    if (!parseSucceeded) return false;

    // Look for "CSeq:", skip whitespace, then read everything up to the next \r or \n as 'CSeq':
    parseSucceeded = false;
    for (j = i; (int)j < (int)(CurRequestSize-5); ++j) 
    {
        if (CurRequest[j]   == 'C' && CurRequest[j+1] == 'S' && 
            CurRequest[j+2] == 'e' && CurRequest[j+3] == 'q' && 
            CurRequest[j+4] == ':') 
        {
            j += 5;
            while (j < CurRequestSize && (CurRequest[j] ==  ' ' || CurRequest[j] == '\t')) ++j;
            unsigned n;
            for (n = 0; n < sizeof(m_CSeq)-1 && j < CurRequestSize; ++n,++j) 
            {
                char c = CurRequest[j];
                if (c == '\r' || c == '\n') 
                {
                    parseSucceeded = true;
                    break;
                }
                m_CSeq[n] = c;
            }
            m_CSeq[n] = '\0';
            break;
        }
    }
    if (!parseSucceeded) return false;

    // Also: Look for "Content-Length:" (optional)
    for (j = i; (int)j < (int)(CurRequestSize-15); ++j) 
    {
        if (CurRequest[j]    == 'C'  && CurRequest[j+1]  == 'o'  && 
            CurRequest[j+2]  == 'n'  && CurRequest[j+3]  == 't'  && 
            CurRequest[j+4]  == 'e'  && CurRequest[j+5]  == 'n'  && 
            CurRequest[j+6]  == 't'  && CurRequest[j+7]  == '-'  && 
            (CurRequest[j+8] == 'L' || CurRequest[j+8]   == 'l') &&
            CurRequest[j+9]  == 'e'  && CurRequest[j+10] == 'n' && 
            CurRequest[j+11] == 'g' && CurRequest[j+12]  == 't' && 
            CurRequest[j+13] == 'h' && CurRequest[j+14] == ':') 
        {
            j += 15;
            while (j < CurRequestSize && (CurRequest[j] ==  ' ' || CurRequest[j] == '\t')) ++j;
            unsigned num;
            if (sscanf(&CurRequest[j], "%u", &num) == 1) m_ContentLength = num;
        }
    }
    return true;
};


};




