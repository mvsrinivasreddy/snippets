
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>    /* Must precede if*.h */
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

union ethframe
{
  struct
  {
    struct ethhdr    header;
    unsigned char    data[ETH_DATA_LEN];
  } field;
  unsigned char    buffer[ETH_FRAME_LEN];
};


int main(int argc, char* argv[])
{

//unsigned short int protocol_type = 0x1234;
unsigned short int protocol_type = 0x0800;
short int sock_desc=0, count=0;




if ( (sock_desc = socket(AF_PACKET, SOCK_RAW, htons(protocol_type))) < 0)
printf("\nError creating network socket\n");
else
{
printf("\nSuccessfully created network socket with descriptor:%d\n", sock_desc);



struct sockaddr_ll sock_addrll;
unsigned char frame_buffer[ETH_FRAME_LEN];
int data_rcvd=0;
socklen_t sock_addrll_len = (socklen_t)sizeof(sock_addrll);
data_rcvd = recvfrom(sock_desc, frame_buffer, ETH_FRAME_LEN, 0x00, (struct sockaddr*) &sock_addrll, &sock_addrll_len); // receive data from the source PC

printf("\nData rcvd (%d):", data_rcvd);
int k=sizeof(struct ethhdr);
for (int i=0;i<data_rcvd;i++)
{
printf("%c", frame_buffer[k]);
k++;
}
printf("\n");


}


return 0;
}
