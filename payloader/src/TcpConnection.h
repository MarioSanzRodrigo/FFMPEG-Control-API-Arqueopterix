#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
 


#define RTSP_BUFFER_SIZE       10000    // for incoming requests, and outgoing responses
#define RTSP_PARAM_STRING_MAX  200  


#include "logger.h"
#include "Interfaces.h"
#include "Streamer.h"

// supported command types
enum RTSP_CMD_TYPES
{
    RTSP_OPTIONS,
    RTSP_DESCRIBE,
    RTSP_SETUP,
    RTSP_PLAY,
    RTSP_TEARDOWN,
    RTSP_PAUSE,
    RTSP_UNKNOWN
};


namespace payloader {
class TcpConnection { 
    DECLARE_LOGGER();
	public:
		TcpConnection(int Client, payloader::Streamer *m_Streamer);
	    long int getPort();
	    void registerParent(PortReceiver *parent);

	    //typedef boost::shared_ptr<TcpConnection> pointer;
	    //boost::shared_ptr<TcpConnection> create(boost::asio::io_service& io_service, payloader::Streamer *m_Streamer);
	    void DateHeader();
	    //tcp::socket& socket();
	    void start();
	    pthread_mutex_t getMutex();
	    virtual ~TcpConnection();

	   
	    int rtspClientPort;
	    boost::array<char, 256> buf; // We use a boost::array to hold the received data. 
	   	//tcp::socket socket_;
		std::string m_message;
	    bool connected = false;   
	 	PortReceiver *myparent;
		char *data;
		RTSP_CMD_TYPES C;
		
		char buf_Date[200]; 
		long int puerto = 0;

		RTSP_CMD_TYPES m_RtspCmdType;  
		 // global session state parameters
		int            m_RtspSessionID;
		int         m_RtspClient;                              // RTSP socket of that session
		int            m_StreamID;                                // number of simulated stream of that session
		u_short        m_ClientRTPPort;                           // client port for UDP based RTP transport
		u_short        m_ClientRTCPPort;    
		u_short         m_RtpServerPort;
		u_short              m_RtcpServerPort;                      // client port for UDP based RTCP transport  
		bool           m_TcpTransport;
		int jumper = 6970; // if Tcp based streaming was activated

		// parameters of the last received RTSP request

		                      // command type (if any) of the current request
		char           m_URLPreSuffix[RTSP_PARAM_STRING_MAX];     // stream name pre suffix 
		char           m_URLSuffix[RTSP_PARAM_STRING_MAX];        // stream name suffix
		char           m_CSeq[RTSP_PARAM_STRING_MAX];             // RTSP command sequence number
		char           m_URLHostPort[RTSP_BUFFER_SIZE];           // host:port part of the URL
		unsigned       m_ContentLength;      
		char   response[1024];                     // SDP string size

		payloader::Streamer* m_Streamer;

		//void handle_read(const boost::system::error_code& error, size_t /*bytes_transferred*/);
		RTSP_CMD_TYPES getC();
		payloader::Streamer* getStreamer();
		RTSP_CMD_TYPES Handle_RtspRequest(char const * aRequest, unsigned aRequestSize);
		
	private:
		//void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);
		
		void Handle_RtspOPTION();
		void Handle_RtspDESCRIBE();
		void Handle_RtspSETUP();
		void Handle_RtspPLAY();
		void Handle_RtspPAUSE();
		bool ParseRtspRequest(char const * aRequest, unsigned aRequestSize);


};
}	// Namespace payloader
#endif // TCPCONNECTION_H