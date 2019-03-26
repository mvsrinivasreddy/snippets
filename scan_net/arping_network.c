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


int arpcache()
{
	int ret=-1,len,i=0;
	char buf[64];
	FILE *fp;
  	char path[1035];
	char IPAddress[16];	
	char *startptr, *endptr;

  //sprintf(buf,"arping -f -I eth0.2 -c 2 %s",targetip);
  system("./sweepnetwork.sh");
  sleep(2);
  sprintf(buf,"cat /proc/net/arp | grep 0x2");
  /* Open the command for reading. */
  fp = popen(buf, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    return -1;
  }

  /* Read the output a line at a time - output it. */
  //10.0.0.6         0x1         0x2         88:51:fb:3e:32:a6     *        enp3s0
  memset(buf,'\0',sizeof(buf));
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("line:%s", path);
    i=0;
    while ((i<16) && (path[i] !=' ')){
	IPAddress[i]=path[i];
	i++;
    }
    IPAddress[i]='\0';
    startptr=strstr(path,"0x2");
    endptr=strstr(path,"*");
    printf("%s\n",startptr);
    printf("%s\n",endptr);
    startptr=startptr+12;
    endptr=endptr-5;
    strncpy(buf, startptr, (endptr-startptr));
    printf("Result : %s, IPAddress:%s,", buf,IPAddress);
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
	
	arpcache();

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
