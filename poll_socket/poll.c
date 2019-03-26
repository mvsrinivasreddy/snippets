#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
//#include <net/if.h>
#include <sys/ioctl.h>
#include <openssl/md5.h>
#include <errno.h>
#include <fcntl.h>

#include <math.h>
#include <signal.h>
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <poll.h>

#define ENPROTO_TCP							0x06


int CheckOptionsCamConn(char *IPAddress)
{
	int 					RetVal=0,Rv=0,status=0;
	int 					sockerror, optlen;
	int						Socket= -1 ;
	char					RecvMessageBuffer [2024] ;
	struct sockaddr_in		RtspAddress ;
	char 					Url[128] ;

	struct pollfd 			ufds;

	struct timeval  timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	memset (&RecvMessageBuffer, 0 , strlen (RecvMessageBuffer)) ;
	memset (&RtspAddress, 0, sizeof (struct sockaddr_in)) ;

	RtspAddress.sin_family = AF_INET ;
	RtspAddress.sin_addr.s_addr = inet_addr(IPAddress) ;
	RtspAddress.sin_port = htons(554);

	if (RtspAddress.sin_port == 0)
	{
		printf("CheckCamConnectivity: Invalid port 0\n") ;
		return 0 ;
	}

//	Socket = socket (AF_INET, SOCK_STREAM | SOCK_NONBLOCK, ENPROTO_TCP) ;
	Socket = socket (AF_INET, SOCK_STREAM, ENPROTO_TCP) ;
	if (Socket < 0)
	{
		printf("CheckCamConnectivity: Failed to create socket: %d\n",errno) ;
		return 0 ;
	}

	struct timeval tv;

	tv.tv_sec = 1;  /* 1 Sec Timeout */
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors

	setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	
	fd_set set;
	FD_ZERO(&set);
	FD_SET(Socket, &set);

	fcntl(Socket, F_SETFL, O_NONBLOCK);

	if ( (status = connect(Socket, (struct sockaddr*)&RtspAddress, sizeof(RtspAddress))) == -1)
	{
		if ( errno != EINPROGRESS )
			return 0;
	}
	status = select(Socket+1, NULL, &set, NULL, &timeout);
	if(status==1 && getsockopt(Socket, SOL_SOCKET, SO_ERROR, &sockerror, &optlen) ==0) {
	    if (sockerror==0){
		    	printf("connect success");
			Rv=1; //successfully connected
		}
		else
		{
		    	printf("connect failed");
			
		}
	}

/*	if (connect (Socket, (struct sockaddr*) &RtspAddress, sizeof (struct sockaddr)) != 0)
	{
		Rv=poll(&ufds,1,2500);
		if ((Rv == -1) || (Rv == 0) ){
			imvglog (ERROR,"CheckCamConnectivity: Failed to connect to %s\n",IPAddress) ;
			RetVal=0;
		}
		else if (connect (Socket, (struct sockaddr*) &RtspAddress, sizeof (struct sockaddr)) != 0)
		{
			imvglog (ERROR,"CheckCamConnectivity: Failed to connect to %s\n",IPAddress) ;
		}
	}
	else*/
	if (Rv)
	ufds.fd = Socket;
	ufds.events = POLLIN; // check for just normal data

	if (connect (Socket, (struct sockaddr*) &RtspAddress, sizeof (struct sockaddr)) != 0)
	{
		Rv=poll(&ufds,1,2500);
		if ((Rv == -1) || (Rv == 0) ){
			printf("CheckCamConnectivity: Failed to connect to after timeout %s\n",IPAddress) ;
			RetVal=0;
		}
		else if (connect (Socket, (struct sockaddr*) &RtspAddress, sizeof (struct sockaddr)) != 0)
		{
			printf("CheckCamConnectivity: Failed to connect to %s\n",IPAddress) ;
		}
	}
	else
	{
		strcpy (RecvMessageBuffer, "OPTIONS") ;
		sprintf(Url,"rtsp://%s:554/ISAPI/streaming/channels/101",IPAddress) ;
		strcat (RecvMessageBuffer, " ") ;
		strcat (RecvMessageBuffer, Url);
		strcat (RecvMessageBuffer, "?auth=YWRtaW46aW1vbml0b3IxMjM=");
		strcat (RecvMessageBuffer, " RTSP/1.0\r\n") ;
		//RtspAddHeader (RecvMessageBuffer, "CSeq", "2005", strlen ("2005")) ;
		strcat (RecvMessageBuffer, "User-Agent: iMVG-RTSP\r\n") ;
		strcat (RecvMessageBuffer, "\r\n") ;

		//printf ("\ncamCheck sent: %s\n",RecvMessageBuffer);
		RetVal = send (Socket, RecvMessageBuffer, strlen(RecvMessageBuffer), 0) ;
		//printf("\nsend ret:%d\n",RetVal);


		memset (&RecvMessageBuffer, 0 , strlen (RecvMessageBuffer)) ;
		RetVal = recv (Socket, RecvMessageBuffer, (2024), MSG_PEEK) ;
		//printf("\nrecv ret:%d\n",RetVal);


		//printf ("\ncamCheck recv: %s\n",RecvMessageBuffer);
	}

	close (Socket) ;

	return RetVal;
}

int main()
{
 	char  IPAddress[16]="10.0.0.10";
	CheckOptionsCamConn(IPAddress);	
}

