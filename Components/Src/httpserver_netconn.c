#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "string.h"
#include "tcpserver_netconn.h"
#include "cmsis_os.h"

#include <stdio.h>

#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )


static void tcp_server_serve(struct netconn *conn) 
{
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;


  while(1){
  printf("receive netconn");
  recv_err = netconn_recv(conn, &inbuf);
  
  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn) == ERR_OK) 
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);
     
	  printf("\n#####################   INCOMING DATA   ####################\n");
	  
//	  printf("incoming data lenght: %d\n\n",buflen);
	  
	  printf("Client: ");
	  for(int i=0;i<buflen;i++)printf("%c",buf[i]);
	  printf("\n");
  
	  char *first = "@SERVER REQUEST:";
	  
	  char *dataRequest = strcat(first,buf); 
	  
	  netconn_write(conn, (const unsigned char*)(dataRequest), sizeof(dataRequest), NETCONN_COPY);
	  
	  netbuf_delete(inbuf);
	}
  /* Close the connection (server closes in HTTP) */
  //netconn_close(conn);
  
  }
  
  }

}


static void tcp_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
 
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 16969);
    
    if (err == ERR_OK)
    {
	   
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
	  
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
		printf("accept netcom\n");
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          tcp_server_serve(newconn);

          /* delete connection */
          //netconn_delete(newconn);
		
        }
      }
    }
  }
}


void tcp_server_netconn_init()
{
  // Здесь происходит создание обработчика запросов к web-серверу 
  sys_thread_new("HTTP", tcp_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}

