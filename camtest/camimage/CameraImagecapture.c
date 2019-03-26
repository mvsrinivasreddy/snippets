/*
 ============================================================================
 Name        : CameraImagecapture.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>


char	IPCamBuffer [2048] ;

int	GetImageCaptureFromCamera ()
{
	int	Status ;
	char   *bufPtr = IPCamBuffer ;
	//IPCameraStruct  *ipc = (IPCameraStruct  *) dev ;

	//if ((strstr (ipc->CamModel, "H264") == NULL) && (strstr(ipc->CamModel,"HIKOUTDOOR") == NULL))

		memset(IPCamBuffer,'\0',2048);

		bufPtr += sprintf (bufPtr, "GET http://10.0.0.117/img/snapshot.cgi") ;
		bufPtr += sprintf (bufPtr, "?size=3&quality=1") ;
		bufPtr += sprintf (bufPtr, " HTTP/1.1\r\n\r\n") ;

//	if((ipc->AuthMode[0] != '\0') && strcmp(ipc ->AuthMode, "AUTH-BASIC") == 0)
//	{

		//Image capture for HIK Vision wireless camera
		bufPtr += sprintf (bufPtr, "GET /Streaming/channels/1/picture") ;
		bufPtr += sprintf (bufPtr, " HTTP/1.1\r\nHost: 10.0.0.117\r\n") ;
		bufPtr += sprintf (bufPtr, "User-Agent: iMVG-Rtsp\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
		bufPtr += sprintf (bufPtr, "Accept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n");
		bufPtr += sprintf (bufPtr, "Connection: keep-alive\r\n");
		bufPtr += sprintf (bufPtr, "Authorization: Basic YWRtaW46aW1vbml0b3IxMjM=\r\n\r\n");
//	}
//	else
////	{
//		bufPtr += sprintf (bufPtr, "GET /cgi-bin/snapshot.cgi?loginuse=admin&loginpas=imonitor123") ;
//		bufPtr += sprintf (bufPtr, " HTTP/1.1\r\nHost:10.0.0.117:80\r\nAuthorization: Basic YWRtaW5pc3RyYXRvcjo=\r\n\r\n") ;
//	}
		printf("IPCamBuffer[%s]\n",IPCamBuffer);


	//Status = SendCommandToIPCamera (ipc->IPAddr, IPCamBuffer, (int) (bufPtr - IPCamBuffer), Buffer, bufferLength, BufferOverFlow, ipc->HTTPPort) ;

	return Status ;
}


int CreateServerConnection(){
	const char* server_name = "10.0.0.193";
	const int 	server_port = 8080;
	char		FileName [32] ;
	char 		Index=0;
	FILE		*fp ;
	int 		n = 0,bytes,Length,BuffSize,IPCTempBufIndex=0,RecvComplete=0;
	int 		len = 0, maxlen = 60000;
	char 		IPCTempBuffer[maxlen];
	char 		RecBuffer[1500];
	char		*pbuffer = IPCTempBuffer;
	char		*tmp;

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	// htons: port in network order format
	server_address.sin_port = htons(server_port);

	// open a stream socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("could not create socket\n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		printf("could not connect to server %d\n", errno);
		return -1;
	}

	printf("Connected to server\n");
	return sock;
}


int Create_ClientForcamera(){
	const char* server_name = "10.0.0.197";
	const int server_port = 80;
	char		FileName [32] ;
	char 		Index=0;
	FILE		*fp ;
	int 		n = 0,bytes,Length,BuffSize,IPCTempBufIndex=0,RecvComplete=0;
	int 		len = 0, maxlen = 60000;
	char 		IPCTempBuffer[maxlen];
	char 		RecBuffer[1500];
	char		*pbuffer = IPCTempBuffer;
	char		*tmp;

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	// htons: port in network order format
	server_address.sin_port = htons(server_port);

	// open a stream socket
	int sock, servsock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

	if (connect(sock, (struct sockaddr*)&server_address,
			sizeof(server_address)) < 0) {
		printf("could not connect to camera\n");
		return 1;
	}

	send(sock, IPCamBuffer, strlen(IPCamBuffer), 0);

	// will remain open until the server terminates the connection
	while ((BuffSize = recv(sock, RecBuffer, 1500, 0)) > 0) {
		
		printf("New Camera IPCTempBufIndex[%d]=%d\n",IPCTempBufIndex,BuffSize);
		memcpy (IPCTempBuffer + IPCTempBufIndex, (char *) RecBuffer, BuffSize) ;
		IPCTempBufIndex += BuffSize ;
		//printf("the buffer Coping=[%s]\n",RecBuffer);
		printf("the buffer Len=[%d]\n",IPCTempBufIndex);
		RecvComplete = 1 ;
	}

	// close the socket
	close(sock);

	if(RecvComplete==1)
	{
		servsock = CreateServerConnection();
		if(servsock != -1){
			send(servsock, IPCTempBuffer, IPCTempBufIndex, 0);
		}
		else{
			printf("Server Connection failed\n");
		}

//		tmp =strstr (IPCTempBuffer,"HTTP/1.1 200 OK");
//		if(tmp != NULL){
//
//			tmp = strstr (tmp, "Content-Length:") ;
//			printf("tmp1[%s]{%d}\n",tmp,strlen(tmp));
//			tmp = strstr (tmp, "Content-Length:") ;
//			tmp += 15 ;	//	Skip "Content-length: "
//			Length = strtol (tmp, NULL, 10) ;
//			//printf("tmp2[%s]\n",tmp);
//			printf("Content length %d***********************************************************************8\n",Length) ;
//
//			//	Loof for start of Payload
//			tmp = strstr (tmp, "\r\n\r\n") ;
//			tmp += 4 ;	//	Ignore "\r\n\r\n"
//			//printf("tmp3[%s]\n",tmp);
//			sprintf (FileName, "Image%d.jpg",Index) ;
//			fp = fopen (FileName, "wb") ;
//			if (fp == NULL)
//			{
//
//				printf("[HIK]Couldn't open file %s to write Image Data\n",FileName) ;
//				return -1 ;
//			}
//			bytes = fwrite (tmp, 1, Length, fp) ;
//
//			if (bytes != Length)
//				printf("[HIK] Error: Could write only %d bytes. But Actual length is %d\n", bytes, Length) ;
//
//			fclose (fp) ;
//		}
	}
	return 0;
}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	GetImageCaptureFromCamera();
	Create_ClientForcamera();

	return EXIT_SUCCESS;
}

