/*
 * recvQ.c
 *
 *  Created on: 19-May-2017
 *      Author: srinivas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>


int connecttcp(char * ip, unsigned short port)
{
	int						Socket,Ret=0;
	struct sockaddr_in 		RemoteAddress;
	unsigned int			RemoteIPAddr=0;
	//struct timeval 			tv;
	int						Reuse=1;

	Socket = socket (AF_INET, SOCK_STREAM, 0x06) ;
	if (Socket < 0)
	{
		printf("CAMGW RTSP Session Socket Creation failed\n");
		//TODO Raise Error and send to SM
		return -1;
	}

	//	tv.tv_sec = 3;  /* 3.5 Secs Timeout */
	//	tv.tv_usec = 500000;  // Not init'ing this can cause strange errors
	//	Ret=setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	//	if(Ret<0)
	//		printf("\n CAMGW SetSocketOpt error:%d\n",errno);
	//	else
	//		printf("\n CAMGW Socket Timeout set for 3.5 secs\n");

	//	if(setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&Reuse, sizeof(Reuse)) < 0)
	//	{
	//		printf("CAMGW-setsockopt() REUSE ADDR error=%d\n",errno);
	//		close(Socket);
	//	}

	printf("IP Address before connect:%s END\n",ip);
	inet_aton (ip, (struct in_addr *) &RemoteIPAddr) ;
	RemoteIPAddr = ntohl(RemoteIPAddr) ;

	RemoteAddress.sin_family = AF_INET ;
	RemoteAddress.sin_addr.s_addr = htonl (RemoteIPAddr) ;
	RemoteAddress.sin_port = htons (port) ;

	if (connect (Socket, (struct sockaddr *) &RemoteAddress, sizeof (struct sockaddr)) != 0)
	{
		close (Socket) ;
		printf("CAMGW RTSP Session Connect failed:%d\n",errno);
		//TODO Raise error and inform SM
		return -1;
	}

	//	Ret=fcntl(Socket, F_SETFL, O_NONBLOCK);
	//	if(Ret<0)
	//		printf("\nCAMGW Set O_NONBLOCK Failed\n");

	return Socket;
}

int checktcpbuffer( int Sock){
	FILE	*proc, *pid, *fp;
	char 	line[32], lscommand[32], lscache[128];
	char	txbuffer[9], rxbuffer[9], inode[6];
	char	fd_inode[6];
	//char filename[16];
	//char 	*ptr= line;
	char 	*ptr= NULL;
	ssize_t read, len=0;

	printf("PID of Buffer scanner : %ld\n",syscall(SYS_gettid));

	sprintf(lscommand,"ls -ilaL /proc/%ld/fd/%d", syscall(SYS_gettid), Sock);

	fp = popen(lscommand, "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
	}

	/* Read the output a line at a time. */
	memset(lscache,'\0',128);
	printf("\n<========== fd Data ==========>\n");
	while (fgets(lscache, sizeof(lscache)-1, fp) != NULL) {
		strncpy(fd_inode, lscache,  5);
		fd_inode[6]='\0';
		printf("%s fd_inode :%s\n", lscache, fd_inode);
	}
	printf("<========== fd Data ==========>\n");

	proc = fopen("/proc/net/tcp","r");
	if (proc == NULL)
		exit(-1);

	while ((read = getline(&ptr, &len, proc)) != -1) {
		//printf("Retrieved line of length %zu with Data :", read);
		snprintf(inode, 6, "%s", (ptr+91));
		if(strcmp(inode, fd_inode) == 0){
			snprintf(txbuffer, 9, "%s:", (ptr+37));
			snprintf(rxbuffer, 9, "%s:", (ptr+46));
			printf("Send-Q: %s | Recv-Q: %s | inode: %s\n", txbuffer, rxbuffer, inode );
		}
	}

	close(Sock);
	fclose(proc);
	pclose(fp);
	return 0;
}

int main(int argc, char *argv[]){
	int Socket;

	Socket = connecttcp("10.0.0.11", 6123);
	if (Socket <= 0)
		printf("connect error\n");
	else
		printf("Socket FD:%d\n",Socket);

	write(Socket,"Hello", 5);
	sleep(5);

	checktcpbuffer(Socket);

}

