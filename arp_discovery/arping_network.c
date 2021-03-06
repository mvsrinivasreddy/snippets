/* Includes */
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
//#include <openssl/md5.h>
#include <errno.h>

#include <fcntl.h>

#include <math.h>
#include <signal.h>
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

/****************************************************/
#define PROTO_ARP 0x0806
#define ETH2_HEADER_LEN 14
#define HW_TYPE 1
#define PROTOCOL_TYPE 0x800
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define ARP_REQUEST 0x01
#define ARP_REPLY 0x02
#define BUF_SIZE 60

struct arp_header
{
        unsigned short hardware_type;
        unsigned short protocol_type;
        unsigned char hardware_len;
        unsigned char  protocol_len;
        unsigned short opcode;
        unsigned char sender_mac[MAC_LENGTH];
        unsigned char sender_ip[IPV4_LENGTH];
        unsigned char target_mac[MAC_LENGTH];
        unsigned char target_ip[IPV4_LENGTH];
};
 /****************************************************/



int ARPScanGetIPfromMACId(unsigned char* source_ip, unsigned char *target_ip, char* MacId)
{
	int sd;
	unsigned char buffer[BUF_SIZE];
	char null_mac=0;
	int RetVal=0;
	char *macptr,*resetptr=MacId;
	//unsigned char source_ip[4] = {10,222,190,160};
	//unsigned char target_ip[4] = {10,222,190,139};
	//unsigned char source_ip[4] = {10,0,0,18};
	//unsigned char target_ip[4] = {10,0,0,41};
	struct ifreq ifr;
	struct ethhdr *send_req = (struct ethhdr *)buffer;
	struct ethhdr *rcv_resp= (struct ethhdr *)buffer;
	struct arp_header *arp_req = (struct arp_header *)(buffer+ETH2_HEADER_LEN);
	struct arp_header *arp_resp = (struct arp_header *)(buffer+ETH2_HEADER_LEN);
	struct sockaddr_ll socket_address;
	int index,ret,length=0,ifindex;

	memset(buffer,0x00,60);
	/*open socket*/
	sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sd == -1) {
		perror("socket():");
		exit(1);
	}
	strcpy(ifr.ifr_name,"eth0.2");
	/*retrieve ethernet interface index*/
	if (ioctl(sd, SIOCGIFINDEX, &ifr) == -1) {
		perror("SIOCGIFINDEX");
		exit(1);
	}
	ifindex = ifr.ifr_ifindex;
	//printf("interface index is %d\n",ifindex);

	/*retrieve corresponding MAC*/
	if (ioctl(sd, SIOCGIFHWADDR, &ifr) == -1) {
		perror("SIOCGIFINDEX");
		exit(1);
	}
	close (sd);

	for (index = 0; index < 6; index++)
	{
		send_req->h_dest[index] = (unsigned char)0xff;
		arp_req->target_mac[index] = (unsigned char)0x00;
		/* Filling the source  mac address in the header*/
		send_req->h_source[index] = (unsigned char)ifr.ifr_hwaddr.sa_data[index];
		arp_req->sender_mac[index] = (unsigned char)ifr.ifr_hwaddr.sa_data[index];
		socket_address.sll_addr[index] = (unsigned char)ifr.ifr_hwaddr.sa_data[index];
	}
	/*printf("Successfully got eth1 MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
			send_req->h_source[0],send_req->h_source[1],send_req->h_source[2],
			send_req->h_source[3],send_req->h_source[4],send_req->h_source[5]);
	printf(" arp_reqMAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
			arp_req->sender_mac[0],arp_req->sender_mac[1],arp_req->sender_mac[2],
			arp_req->sender_mac[3],arp_req->sender_mac[4],arp_req->sender_mac[5]);
	printf("socket_address MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
			socket_address.sll_addr[0],socket_address.sll_addr[1],socket_address.sll_addr[2],
			socket_address.sll_addr[3],socket_address.sll_addr[4],socket_address.sll_addr[5]);
	*/
	/*prepare sockaddr_ll*/
	socket_address.sll_family = AF_PACKET;
	socket_address.sll_protocol = htons(ETH_P_ARP);
	socket_address.sll_ifindex = ifindex;
	socket_address.sll_hatype = htons(ARPHRD_ETHER);
	socket_address.sll_pkttype = (PACKET_BROADCAST);
	socket_address.sll_halen = MAC_LENGTH;
	socket_address.sll_addr[6] = 0x00;
	socket_address.sll_addr[7] = 0x00;

	/* Setting protocol of the packet */
	send_req->h_proto = htons(ETH_P_ARP);

	/* Creating ARP request */
	arp_req->hardware_type = htons(HW_TYPE);
	arp_req->protocol_type = htons(ETH_P_IP);
	arp_req->hardware_len = MAC_LENGTH;
	arp_req->protocol_len =IPV4_LENGTH;
	arp_req->opcode = htons(ARP_REQUEST);
	for(index=0;index<5;index++)
	{
		arp_req->sender_ip[index]=(unsigned char)source_ip[index];
		arp_req->target_ip[index]=(unsigned char)target_ip[index];
	}
	// Submit request for a raw socket descriptor.
	if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
		perror ("socket() failed ");
		exit (EXIT_FAILURE);
	}

	buffer[32]=0x00;
	ret = sendto(sd, buffer, 42, 0, (struct  sockaddr*)&socket_address, sizeof(socket_address));
	if (ret == -1)
	{
		perror("sendto():");
		exit(1);
	}
	else
	{
		//printf(" Sent the ARP REQ \n\t");
		/*for(index=0;index<42;index++)
		{
			printf("%02X ",buffer[index]);
			if(index % 16 ==0 && index !=0)
			{printf("\n\t");}
		}*/
	}
	//printf("\n\t");
	memset(buffer,0x00,60);
	int count = 0;
	while(1)
	{
		length = recvfrom(sd, buffer, BUF_SIZE, 0, NULL, NULL);
		if (length == -1)
		{
			perror("recvfrom():");
			exit(1);
		}
		if(htons(rcv_resp->h_proto) == PROTO_ARP)
		{
			count++;
			macptr=MacId;
			//if( arp_resp->opcode == ARP_REPLY )
			/*printf(" RECEIVED ARP RESP len=%d \n",length);
			printf(" Sender IP :");
			for(index=0;index<4;index++)
				printf("%u.",(unsigned int)arp_resp->sender_ip[index]);
			i*/
			//printf("\n Sender MAC :");
			for(index=0;index<6;index++)
			{
				if (index<=4){
					//printf(" %02X:",arp_resp->sender_mac[index]);
					macptr+=sprintf(macptr,"%02x:", arp_resp->sender_mac[index]);
				}
				else{
					//printf(" %02X",arp_resp->sender_mac[index]);
					macptr+=sprintf(macptr,"%02x", arp_resp->sender_mac[index]);
				}
			}

			/*printf("\nReceiver  IP :");
			for(index=0;index<4;index++)
				printf(" %u.",arp_resp->target_ip[index]);*/
			//printf("\n Self MAC :");
			for(index=0;index<6;index++)
			{
				if (arp_resp->target_mac[index] == 0x00)
					null_mac=null_mac+1;
				//printf(" %02X:",arp_resp->target_mac[index]);
			}

			printf("\n");
			if (null_mac >= 5)
			{
				//printf("Failed : No response for %hhu\n", target_ip[3]);
				RetVal=1;
			}
			else
			{
				printf("GOT MAC ID:%s\n",resetptr);
				break;
			}
			if(count>=5)
				break;
			else
			{
				null_mac=0;
				macptr=resetptr;
				printf(" [try %d] ",count);
			}
		}	
	}
	close(sd);
	return RetVal;	
}

int arping(char* targetip, char* MacId)
{
	int ret=-1;
	char buf[64];
	FILE *fp;
  	char path[1035];

  //sprintf(buf,"arping -f -I eth0.2 -c 2 %s",targetip);
  sprintf(buf,"arping -f -I enp3s0 -c 2 %s",targetip);
  /* Open the command for reading. */
  fp = popen(buf, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    return -1;
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("line:%s", path);
  }

  /* close */
  pclose(fp);

  return 0;
}

void main()
{
	int 					RetVal=0, Index, IpCamIndex=0;
	char					ImvgWanIP[32]="10.0.0.35", IPAddress[16] ;
    	char 					MacId[32]="";
    	unsigned char 			SourceIp[4],ip,TargetIp[4];
    //DeviceDataStruct		*Dev;
    //IPCameraStruct			*ipc;

    //No need to run more than one scan thread at a time
    //IPScanInProgress=1;
	//GetImvgWanIPAddress (ImvgWanIP) ;
	sscanf(ImvgWanIP,"%hhu.%hhu.%hhu.%hhu", &SourceIp[0], &SourceIp[1], &SourceIp[2], &SourceIp[3]);
	sscanf(ImvgWanIP,"%hhu.%hhu.%hhu", &TargetIp[0], &TargetIp[1], &TargetIp[2]);

	//TriggerWatchdog() ;

	/*for (Index = 0; Index < MAX_DEVICES_SUPPORTED; Index++)
	{
		Dev = GetDeviceDataInstance (Index) ;

		if (Dev->DeviceType == DEV_IP_CAMERA)
		{
			ipc = (IPCameraStruct  *) Dev->DeviceConfigStruct ;
			strcpy(IPCamList[IpCamIndex].CameraDevId,ipc->deviceId);
			strcpy(IPCamList[IpCamIndex].CameraIP,ipc->IPAddress);
			IPCamList[IpCamIndex].DataStructIndex=Index;
			IpCamIndex++;
		}
	}
	printf ("No of cameras Added:%d\n",IpCamIndex);*/

	printf("\n\n\n");
	system("date");
	printf("\n\n\n");

	for (ip=2;ip<=15;ip++)
	{
		TargetIp[3]=ip;
		printf("Querying IP : %hhu.%hhu.%hhu.%hhu\n", TargetIp[0],TargetIp[1],TargetIp[2],TargetIp[3]);
		sprintf(IPAddress,"%hhu.%hhu.%hhu.%hhu", TargetIp[0],TargetIp[1],TargetIp[2],TargetIp[3]);
		//RetVal=ARPScanGetIPfromMACId(SourceIp, TargetIp, MacId);
		RetVal=arping(IPAddress,MacId);
		if (RetVal==0)
		{
			snprintf(IPAddress,sizeof(IPAddress), "%hhu.%hhu.%hhu.%hhu", TargetIp[0],TargetIp[1],TargetIp[2],TargetIp[3]);
			printf ("\nGot ARP response for : %s, MAc ID: %s\n",IPAddress, MacId);

			/*printf("IpIndex: %d Checking with Device IDs:",IpCamIndex);
			for (Index=0;Index<=IpCamIndex;Index++)
			{
				printf("<< %s >>", IPCamList[Index].CameraDevId);
				1if (strstr(IPCamList[Index].CameraDevId, MacId) != NULL)
				{
					printf("\nCamera IP Found: %s\n", IPAddress);
					strcpy(IPCamList[Index].CameraIP, IPAddress);
					ipc = (IPCameraStruct  *) DeviceData[IPCamList[Index].DataStructIndex].DeviceConfigStruct;
					strcpy(ipc->IPAddress, IPAddress);
				}
			}*/
		}
		sleep(2);
	}

	printf("\n\n\n");
	system("date");
	printf("\n\n\n");

	if (ip>=15)
	{
		printf("Camera not found\n");
		RetVal=1;
	}

	//IPScanInProgress=0;
	printf("Camera Scan IP Thread Exit\n");

	return;
}
