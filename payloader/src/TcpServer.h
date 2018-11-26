#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "TcpConnection.h"
#include "Interfaces.h"
#include "logger.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include "pevents.h"
#include <chrono>
#include <thread>
#include <functional>         
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <assert.h>
#include <atomic>
#include <signal.h>
#include <random>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros


using namespace std;
using namespace boost::system;
//using namespace neosmart;

namespace payloader {

typedef void* HANDLE;

class TcpServer : public PortReceiver {
    DECLARE_LOGGER();
	public:
		TcpServer();
		virtual ~TcpServer();
		long int getterData();
		void tomaPuerto(long int puerto);
		
		char* data;
	    long int puerto;
	    pthread_t mithread;
	   
 

	private:
		void start_accept();
		//void handle_accept(TcpConnection::pointer new_connection, const boost::system::error_code& error);

};
}	// Namespace payloader
#endif // TCPSERVER_H