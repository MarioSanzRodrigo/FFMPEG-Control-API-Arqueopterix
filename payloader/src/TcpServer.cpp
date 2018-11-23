#include "TcpServer.h"
//using namespace neosmart;

namespace payloader {

 typedef struct parametrosThread2
  {
    int   Client;
    fd_set descriptoresLectura;
    neosmart_event_t  evento;
  }paramThread2;


void timer_start(std::function<void(neosmart_event_t)> func, unsigned int interval,  neosmart_event_t event){
    std::thread([func, interval, event]() {
        while (true)
        {
            func(event);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}
void do_something(neosmart_event_t event){
    //std::cout << "I am doing something" << std::endl;//activar el evento 
    SetEvent(event);
}

//std::function<void(neosmart_event_t)> func = do_something; Envolotorio que se aplica al pasar el parametro
void* thread_function02(void* myparamThread) {

  paramThread2 *myparamThread2 = (paramThread2*)myparamThread;
  int Client = myparamThread2 -> Client;
  fd_set descriptoresLectura = myparamThread2 -> descriptoresLectura;
  neosmart_event_t event = myparamThread2 -> evento;

  while(true){
    //printf("Hebra 2 pensando...\n");
    select (Client+1, &descriptoresLectura, NULL, NULL, NULL);
    if (FD_ISSET (Client, &descriptoresLectura)){
      //printf("Evento de negociacion.\n");
       SetEvent(event);
       FD_ZERO (&descriptoresLectura); 
       FD_SET (Client, &descriptoresLectura); 
    }
  }
}

void* thread_function01(void* clientSockect) {

  paramThread2 parametros;
  pthread_t mithread2;
  bool Stop = false;
  bool StreamingStarted = false;
  int Client = *(int*)clientSockect;
  char         RecvBuf[10000];                    // receiver buffer
  int          res;  
  payloader::Streamer* m_Streamer = new payloader::Streamer();                  // our streamer for UDP/TCP based RTP transport
  payloader::TcpConnection* new_connection = new payloader::TcpConnection(Client, m_Streamer);     // our threads RTSP session and state
  neosmart_event_t events[2];
  events[0] = CreateEvent(false, false); //comprobación periódica     bool manualReset, bool initialState
  events[1] = CreateEvent(false, false); //abort manual-reset event

  timer_start(do_something, 1000, events[0]);//Aqui hay que pasarle el evento para setevent periodicamente

  fd_set descriptoresLectura; 
  FD_ZERO (&descriptoresLectura); 
  FD_SET (Client, &descriptoresLectura); 

  parametros.Client = Client;
  parametros.descriptoresLectura = descriptoresLectura;
  parametros.evento = events[1];
  
  pthread_create( &mithread2, NULL, &thread_function02,  &parametros);

  while (!Stop){

    int index = -1;
    WaitForMultipleEvents(events,2, false, -1, index);//Index indica si ha pasado un evento u otro, ESPERA INFINITA
    switch (index){//Espera a que suceda algún evento, recivir por el socket o la comprobacion periodica
        case 1 : 
        {   
          //printf("Index: %d\n", index); 
          //printf("Negociando...\n");
            // read client socket
            ResetEvent(events[1]);

            memset(RecvBuf,0x00,sizeof(RecvBuf));
            res = recv(Client,RecvBuf,sizeof(RecvBuf),0);

            // we filter away everything which seems not to be an RTSP command: O-ption, D-escribe, S-etup, P-lay, T-eardown
            if ((RecvBuf[0] == 'O') || (RecvBuf[0] == 'D') || (RecvBuf[0] == 'S') || (RecvBuf[0] == 'P') || (RecvBuf[0] == 'T'))
            {
                RTSP_CMD_TYPES C = new_connection->Handle_RtspRequest(RecvBuf,res);
                if (C == RTSP_PLAY)     StreamingStarted = true; else if (C == RTSP_TEARDOWN) Stop = true; /*else if (C == RTSP_PAUSE) StreamingStarted = false;*/
            };
            break;      
        };
        case 0 : 
        {   
            //printf("Index: %d\n", index); 
            //printf("Comprobando...\n");
            if (StreamingStarted) m_Streamer->StartStreaming();
            break;
        };
    };
  };
    close(Client);
    return 0;
}


DEFINE_LOGGER(TcpServer, "TcpServer");
// Constructor: initialises an acceptor to listen on TCP port 8554.
TcpServer::TcpServer(){//Creamos el soocket en el puerto 8554 escuchando tcp para la conexión rtsp
  start_accept();
}
TcpServer::~TcpServer(){
    //io_service.stop();
}

void TcpServer::tomaPuerto(long int puerto){
      this->puerto = puerto;
}
long int TcpServer::getterData(){
      return puerto;
}
void TcpServer::start_accept(){
  int      MasterSocket;                                 // our masterSocket(socket that listens for RTSP client connections)  
  int      ClientSocket;                                 // RTSP socket to handle an client
  sockaddr_in ServerAddr;                                   // server address parameters
  sockaddr_in ClientAddr;                                   // address parameters of a new RTSP client
  socklen_t         ClientAddrLen = sizeof(ClientAddr);   
  ServerAddr.sin_family      = AF_INET;   
  ServerAddr.sin_addr.s_addr = INADDR_ANY;   
  ServerAddr.sin_port        = htons(8554);                 // listen on RTSP port 8554
  MasterSocket               = socket(AF_INET,SOCK_STREAM,0);   
  // creates a socket

  // bind our master socket to the RTSP port and listen for a client connection
    if (bind(MasterSocket,(sockaddr*)&ServerAddr,sizeof(ServerAddr)) != 0) return;  
    if (listen(MasterSocket,5) != 0) return;

     while (true)  
    {   // loop forever to accept client connections
        ClientSocket = accept(MasterSocket,(struct sockaddr*)&ClientAddr,&ClientAddrLen);   
         
        //CreateThread(NULL,0,thread_function01,&ClientSocket,0);
        pthread_create( &mithread, NULL, &thread_function01,  &ClientSocket);
        printf("Client connected. Client address: %s\r\n",inet_ntoa(ClientAddr.sin_addr));  
    }  

    close(MasterSocket);   
  }

}  
