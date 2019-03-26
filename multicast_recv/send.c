/*multicast.c

The following program sends or receives multicast packets. If invoked
with one argument, it sends a packet containing the current time to an
arbitrarily chosen multicast group and UDP port. If invoked with no
arguments, it receives and prints these packets. Start it as a sender on
just one host and as a receiver on all the other hosts

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>
#include<uuid/uuid.h>

#define EXAMPLE_PORT 3702
#define SRC_PORT 3702
#define EXAMPLE_GROUP "239.255.255.250"

int main(int argc)
{

	// typedef unsigned char uuid_t[16];
        uuid_t uuid;

        // generate
        uuid_generate_time_safe(uuid);
        char uuid_str[37];      // ex. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + "\0"
        uuid_unparse_lower(uuid, uuid_str);
        printf("generate uuid=%s\n", uuid_str);

   struct sockaddr_in addr,srcaddr,recvaddr;
   int addrlen, sock, cnt, rsock;
   struct ip_mreq mreq;
   char message[3024];
   char sender[64];

   /* set up socket */
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) {
     perror("socket");
     exit(1);
   }
	
   /* set up socket */
   rsock = socket(AF_INET, SOCK_STREAM, 0x06);
   if (sock < 0) {
     perror("socket");
     exit(1);
   }
 
   bzero((char *)&addr, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(EXAMPLE_PORT);
   addrlen = sizeof(addr);
   
   //recv data
   bzero((char *)&recvaddr, sizeof(recvaddr));
   recvaddr.sin_family = AF_INET;
   recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   recvaddr.sin_port = htons(SRC_PORT);

     int enable = 1;
     if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
     	printf("setsockopt(SO_REUSEADDR) failed\n");
   
	int flags = fcntl(sock,F_GETFL,0);
	assert(flags != -1);
	printf("Set non blocking : %d\n",fcntl(sock, F_SETFL, flags | O_NONBLOCK));
	
	/*struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout); */

    if (bind(sock, (struct sockaddr *) &srcaddr, sizeof(srcaddr)) < 0) {
        perror("bind");
        exit(1);
    }

	
      /* send */
      addr.sin_addr.s_addr = inet_addr(EXAMPLE_GROUP);
      //while (1) {
         uuid_generate_random(uuid);
         uuid_unparse_lower(uuid, uuid_str);
         printf("\ngenerate uuid=%s\n", uuid_str);
	sprintf(message, "<?xml version=\"1.0\" encoding=\"utf-8\"?><Probe><Uuid>%s</Uuid><Types>inquiry</Types></Probe>",uuid_str);
	 //sprintf(message, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><e:Envelope xmlns:e=\"http://www.w3.org/2003/05/soap-envelope\"xmlns:w=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\"xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\"><e:Header><w:MessageID>uuid:%s</w:MessageID><w:To e:mustUnderstand=\"true\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</w:To><w:Actiona:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</w:Action></e:Header><e:Body><d:Probe><d:Types>dn:NetworkVideoTransmitter</d:Types></d:Probe></e:Body></e:Envelope>",uuid_str);
	 printf("\nsending: %s\n", message);
	 cnt = sendto(sock, message, strlen(message), 0,
		      (struct sockaddr *) &addr, sizeof(addr));
	 if (cnt < 0) {
 	    perror("sendto");
	    exit(1);
	 }
	 usleep(20000);
	
	 for (int i=0;i<6;i++){
 	 cnt = recvfrom(sock, message, sizeof(message), 0, 
	 		(struct sockaddr *) &recvaddr, &addrlen);
	 if ((cnt < 0)) {
	    perror("recvfrom");
	    //exit(1);
	 }
	 else{
	 	inet_ntop(AF_INET, &recvaddr.sin_addr, sender, sizeof sender);
	 	printf("\nRecv from: %s\n",sender);
	 	printf("\nrecieved MSG: %s\n", message);
	 	memset(message,'\0', sizeof(message));
   	 	bzero((char *)&recvaddr, sizeof(recvaddr));
	 }
	 }
	 sleep(1);
   close(sock); 
}

