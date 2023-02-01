#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "string.h"
#include "tcpserver_netconn.h"
#include "cmsis_os.h"
#include "main.h"

#include "PRLV_STVC.h"
#include "PRLV_STVC_ETHERNET.h"


#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )

uint32_t dataBuffer[16];



static void tcp_server_serve(struct netconn *conn) 
{
  struct netbuf *inbuf;
  err_t recv_err;
  uint8_t* buf;
  u16_t buflen;
  uint8_t  dataResponce[64];
  

  while(1)
  {
  	recv_err = netconn_recv(conn, &inbuf); 
  	if (recv_err == ERR_OK)
  	{
		err_t netconnError = netconn_err(conn);
    	if (netconnError == ERR_OK) 
    	{
      	netbuf_data(inbuf, (void**)&buf, &buflen);
		memcpy(dataBuffer,buf,buflen);	
		
     	prlv_header_struct* Header = (prlv_header_struct*) dataBuffer;
		
		
	  	printf("\n~~~~~INCOMING DATA~~~~~\n");
//		printf("	Range of data: %d byte\n",buflen);	  		
//		printf("	Header: 0x%x\n",Header->header);
//		printf("	Lenght: %d\n",Header->lenght);
//		printf("	ID    : %d\n",Header->id);
		printf("	Command: ");	
		if(Header->header != PROTOCOL_HEADER){printf("HEADER ERROR");break;}
		
		uint32_t responceLength=10;
		for(int i=0;i<responceLength;i++)dataResponce[i]=1;
		
		switch (Header->id){
			case PROTOCOL_ID_GETSTATES:
			  responceGZ(dataResponce);//responceRTNPA(dataResponce);
			  responceLength = 20 + 8; 	   //responceLength = 36 + 8;
			  	break;		  
			case PROTOCOL_ID_SETMOTORS:
			  	setMotors(dataBuffer,dataResponce); 
			  	responceLength =0 + 8;
			  	break;
			case PROTOCOL_ID_SETOFFMOTORS:
			  setOffsetMotors(dataBuffer);
			  responceLength=0+ 8;
			  	break;
			case PROTOCOL_ID_SETLAMPS:
			  setLamps(dataBuffer,dataResponce);  
			  responceLength = 0 + 8;
			  	break;
			case PROTOCOL_ID_SETLASER:
			  setLaser(dataBuffer,dataResponce);
			  responceLength = 0 + 8;
			  	break;
			case PROTOCOL_ID_CALIBRATION:
			  setCalibration(dataBuffer,dataResponce);
			  responceLength = 0 + 8;
			  	break;
		}
		  
		if(Header->id != PROTOCOL_ID_SETOFFMOTORS){
	  	printf("	-Sending responce-\n");
	  	netconn_write(conn, (const unsigned char*)(dataResponce), responceLength, NETCONN_COPY);
		}
		
	  	netbuf_delete(inbuf);
		
	
		
		
		
		}else if(netconnError!=-1){	// Исключаем ошибку буффера
	  		printf("		@ netconnError =%d\n",netconnError); 
		} // это нужно чтобы отловить какую-то лажу с работой уже с подкл. клиентом	
  	}else{
		if(recv_err !=-1){
			if(recv_err==-11){
	  			printf("~~~~~DISCONNECTED~~~~~\n");
				RGBsignal_OK(0);
				
				//netconn_delete(conn);
	  			break;
			}//else if(recv_err==-13)printf("		@connection aborted\n");
			//else if(recv_err==-14)printf("		@connection reset\n");
			//else if(recv_err==-15)printf("		@connection closed\n");
			//else printf("		@recv_err = %d\n",recv_err);
  		}
  	}
  }
}




static void tcp_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;

while(1){//%%%

  /* Create a new TCP connection handle */
//  printf("1NETCONN NEW\n");
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
	
//	printf("	2NETCONN BIND\n");
	err = netconn_bind(conn, NULL, SERVER_PORT);
//    printf("end bind\n");
    if (err == ERR_OK)
    {
	   
      /* Put the connection into LISTEN state */
//	  printf("start listen\n");
	  
//      printf("		3NETCONN LISTEN\n");
	  netconn_listen(conn);
//	  printf("end listen\n");
	  

        /* accept any icoming connection */
//        printf("			4NETCONN ACCEPT\n");
		accept_err = netconn_accept(conn, &newconn);
		
		ip_addr_t clientAddr;
		u16_t clientPort;
		netconn_getaddr(newconn, &clientAddr, &clientPort, 0);
		printf("\n\n~~~~~NEW CONNECTION~~~~~\n");
		printf("Address:   ");ip4_addr_debug_print(LWIP_DBG_ON,&clientAddr);
		printf("\nLocal port:   %d\n",clientPort);
		printf("------------------------\n");
		RGBsignal_OK(1);
		

//		ip4_addr_debug_print_parts(LWIP_DBG_ON,
		

        if(accept_err == ERR_OK)
        {
          /* serve connection */
//		  printf("start serve\n");
//		  printf("			5NETCONN SERVE\n");
          tcp_server_serve(newconn);
//		  printf("end serve\n");

          /* delete connection */
          netconn_delete(newconn);
		  netconn_delete(conn);

		
		}else{
		 printf("accept_err = %d\n",accept_err); 
		}
		
      
	}else{
	  //printf("ERR = %d\n",err);
	  //netconn_delete(newconn);
	  netconn_delete(conn);
//	  break;
	}
  }
  
  
}//%%%
}


void tcp_server_netconn_init()
{
  // Здесь происходит создание обработчика запросов к web-серверу 
  sys_thread_new("HTTP", tcp_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}

