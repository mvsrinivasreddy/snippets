/* ------------------------------------------------------------ *
 * file:        sslconnect.c                                    *
 * purpose:     Example code for building a SSL connection and  *
 *              retrieving the server certificate               *
 * author:      06/12/2012 Frank4DD                             *
 *                                                              *
 * gcc -o sslconnect sslconnect.c -lssl -lcrypto                *
 * ------------------------------------------------------------ */

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#define MAX_RECV_BUF_LEN 1024
#define URL  "https://www.imssmartcontrol.com"
#define PORT "2920"

typedef enum{
	IMVG_REGISTER,
	RUN_IMVG,
	IMVG_KEEPALIVE,
	IMVG_UP
}IMVG_COMMANDS;

int CMSConnectionStatus=0;
BIO              *certbio = NULL;
BIO               *outbio = NULL;
X509                *cert = NULL;
X509_NAME       *certname = NULL;
const SSL_METHOD *method;
SSL_CTX *ctx;
SSL *ssl;
int server = 0;
char	host_addr[256] = "";
int 	argport=0;

void 	GenTransId (char *Buffer, int *BufferLen)
{
	int  randomNum = random () ;

    memset (Buffer, 0, *BufferLen) ;
    if(*BufferLen < 9)
    {
    	*BufferLen = 0 ;
    	return ;
    }

    sprintf (Buffer, "%x", randomNum) ;
    *BufferLen = strlen (Buffer) ;
}

void GetTimeStamp (char *TimeStamp)
{
	time_t     	CurrTime ;
	struct	tm 	*TS ;
	char       	TempBuff [64] ;

	TimeStamp [0] = '\0' ;
	TempBuff [0] = '\0' ;

	CurrTime = time (NULL) ;
	TS = localtime (&CurrTime) ;
	strftime (TempBuff, sizeof (TempBuff), "%Y-%m-%d %H:%M:%S", TS) ;

	strcpy (TimeStamp, TempBuff) ;
}

int	 SendMsgToCMS (char *MsgBuffer, int MsgBufferLen)
{
	int				RetVal = 0;
	//char			TransId [MAX_TRANSACTION_ID_SIZE] ;
	int				Index;
	unsigned short 	BSize;

	MsgBuffer [MsgBufferLen] = '\0' ;
	//imvglog (DEBUG," Sent data (%d bytes) : %s \n", MsgBufferLen, MsgBuffer) ;
	printf(" Sent data (%d bytes) : %s \n", MsgBufferLen, MsgBuffer) ;

	BSize = htons (MsgBufferLen) ;
	for(Index = MsgBufferLen + 1; Index >= 2; Index--){
		MsgBuffer [Index] = MsgBuffer [Index - 2] ;
	}
	memcpy (MsgBuffer, &BSize, 2) ;
	MsgBufferLen += 2 ;
	MsgBuffer[MsgBufferLen+1]='\0';


	if(CMSConnectionStatus){
		RetVal = SSL_write(ssl, MsgBuffer, MsgBufferLen);
		if (RetVal <= 0) {
			printf("failed SSL write KeepAlive Task, return [-0x%x]\n", -RetVal);
			RetVal = -1;
		}
	}
	else{
		printf("iMVG not connected to CMS. Message not sent\n");
		RetVal = -1;
	}
	return RetVal ;
}

/* ---------------------------------------------------------- *
 * First we need to make a standard TCP socket connection.    *
 * create_socket() creates a socket & TCP-connects to server. *
 * ---------------------------------------------------------- */
int create_socket(char[], BIO *);


int sslconnect()
{

	char  dest_url[] = URL;

	certbio = NULL;
	outbio = NULL;
	cert = NULL;
	certname = NULL;



	/* ---------------------------------------------------------- *
	 * These function calls initialize openssl for correct work.  *
	 * ---------------------------------------------------------- */
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();

	/* ---------------------------------------------------------- *
	 * Create the Input/Output BIO's.                             *
	 * ---------------------------------------------------------- */
	certbio = BIO_new(BIO_s_file());
	outbio  = BIO_new_fp(stdout, BIO_NOCLOSE);

	/* ---------------------------------------------------------- *
	 * initialize SSL library and register algorithms             *
	 * ---------------------------------------------------------- */
	if(SSL_library_init() < 0)
		BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");

	/* ---------------------------------------------------------- *
	 * Set SSLv2 client hello, also announce SSLv3 and TLSv1      *
	 * ---------------------------------------------------------- */
	method = SSLv23_client_method();

	/* ---------------------------------------------------------- *
	 * Try to create a new SSL context                            *
	 * ---------------------------------------------------------- */
	if ( (ctx = SSL_CTX_new(method)) == NULL)
		BIO_printf(outbio, "Unable to create a new SSL context structure.\n");

	/* ---------------------------------------------------------- *
	 * Disabling SSLv2 will leave v3 and TSLv1 for negotiation    *
	 * ---------------------------------------------------------- */
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

	/* ---------------------------------------------------------- *
	 * Create new SSL connection state object                     *
	 * ---------------------------------------------------------- */
	ssl = SSL_new(ctx);

	/* ---------------------------------------------------------- *
	 * Make the underlying TCP socket connection                  *
	 * ---------------------------------------------------------- */
	server = create_socket(dest_url, outbio);
	if(server > 0)
		BIO_printf(outbio, "Successfully made the TCP connection to: %s.\n", dest_url);
	else
		return -1;

	/* ---------------------------------------------------------- *
	 * Attach the SSL session to the socket descriptor            *
	 * ---------------------------------------------------------- */
	SSL_set_fd(ssl, server);

	/* ---------------------------------------------------------- *
	 * Try to SSL-connect here, returns 1 for success             *
	 * ---------------------------------------------------------- */
	if ( SSL_connect(ssl) != 1 ){
		BIO_printf(outbio, "Error: Could not build a SSL session to: %s.\n", dest_url);
		return -1;
	}
	else
		BIO_printf(outbio, "Successfully enabled SSL/TLS session to: %s.\n", dest_url);

	/* ---------------------------------------------------------- *
	 * Get the remote certificate into the X509 structure         *
	 * ---------------------------------------------------------- */
	cert = SSL_get_peer_certificate(ssl);
	if (cert == NULL)
		BIO_printf(outbio, "Error: Could not get a certificate from: %s.\n", dest_url);
	else
		BIO_printf(outbio, "Retrieved the server's certificate from: %s.\n", dest_url);

	/* ---------------------------------------------------------- *
	 * extract various certificate information                    *
	 * -----------------------------------------------------------*/
	certname = X509_NAME_new();
	certname = X509_get_subject_name(cert);

	/* ---------------------------------------------------------- *
	 * display the cert subject here                              *
	 * -----------------------------------------------------------*/
	BIO_printf(outbio, "Displaying the certificate subject data:\n");
	X509_NAME_print_ex(outbio, certname, 0, 0);
	BIO_printf(outbio, "\n");

	CMSConnectionStatus=1;
	return 0;
}

int main(int argc,char* argv[])
{
	int counter=1;
	int  timeout=0;

	int ret, i;
	char   Buffer[1024];
	char   sendbuf[1024];
	char   recvbuf[1024];
	int	 len=0;
	char	TranscationId[20] ;
	int TranscationIdLen=0 ;
	int 	Periodic_KA =0;
	int 	Command=-1;
	char   TimeStamp [64] ;

	if(argc==1){
		printf("No arguments passed. You need to pass arguments as shown below:\n\nFormat:[gwstub <MAC_ID> <COMMAND> <TIMEOUT> <HOSTNAME> <PORT>]\n\nExample:[gwstub 00:0e:8f:65:7c:e2 IMVG_UP 5 www.imssmartcontrol.com 2920]\n\n");

		printf(" Commands Supported:\n 1. IMVG_UP\n 2. IMVG_REGISTER\n 3. KEEP_ALIVE\n 4. RUN_IMVG\n\n");
		return 0;
	}
	if(argc>2)
	{
		printf("\nNumber Of Arguments Passed: %d",argc);
		printf("\n----Following Are The Command Line Arguments Passed----");
		//for(counter=0;counter<argc;counter++)
		// printf("\nargv[%d]: %s",counter,argv[counter]);
		printf("\n%d MAC_ID: %s",counter,argv[counter++]);
		printf("\n%d COMMAND: %s",counter,argv[counter++]);
	}
	else{
		printf("Not enough arguments\n");
	}

	if(argc >=3){
		printf("\n%d TIMEOUT: %s",counter,argv[counter++]);
		printf("\n");
		if((atoi(argv[3]) <= 0) || (atoi(argv[3]) > 60)){
			printf("ERROR: Invalid Timeout select a value from 1 to 60 seconds");
		}
		else
			timeout = atoi(argv[3]);
	}

	if(argc>=5){
		printf("\n%d IP: %s",counter,argv[counter++]);
		printf("\n%d PORT: %s",counter,argv[counter++]);
		printf("\n");
		strcpy(host_addr, argv[4]);
		argport = atoi(argv[5]);
	}
	else
		host_addr[0]='\0';


Connect:
	ret = sslconnect();
	if(ret < 0){
		printf("Trying again after 3 seconds\n");
		sleep(3);
		goto Connect;
	}
	else
		printf("SSL COnnectivity success\n");


	if(strstr(argv[2], "IMVG_REGISTER") > 0){
		Command = IMVG_REGISTER;
	}
	else if(strstr(argv[2], "IMVG_UP") > 0){
		Command = IMVG_UP;
	}
	else if(strstr(argv[2], "KEEP_ALIVE") > 0){
		Command = IMVG_KEEPALIVE;
	}
	else if(strstr(argv[2], "RUN_IMVG") > 0){
		printf("\n\n RUN_IMVG Selected \n\n");
		Command = RUN_IMVG;
	}

	while(1){

		//Trans Id
		TranscationIdLen = 20 ;
		memset (TranscationId, 0, TranscationIdLen) ;
		GenTransId (TranscationId, &TranscationIdLen) ;


		if(Periodic_KA){
			Command=IMVG_KEEPALIVE;
		}

		switch(Command){
		case IMVG_UP:
			GetTimeStamp (TimeStamp) ;
			TimeStamp[63] = '\0';
			sprintf(Buffer,"CMD_ID=DEVICE_ALERT\r\nTRANSACTION_ID=%s\r\nIMVG_ID=%s\r\nDEVICE_ID=%s\r\nALERT_TYPE=ACTIVE\r\nALERT_EVENT=IMVG_UP\r\nIMVG_TIME_STAMP=%s\r\nUSR_DTLS=MAIN_USR\r\nIMVG_VERSION=IMSPRIME_1.0.2_15_02_2018\r\nSIGNATURE=0x3d66\r\n",TranscationId, argv[1], argv[1], TimeStamp);
			break;

		case IMVG_REGISTER:
			sprintf(Buffer, "CMD_ID=IMVG_REGISTER\r\nTRANSACTION_ID=%s\r\nIMVG_ID=%s\r\nSIGNATURE=0x1385\r\n\r\n", TranscationId, argv[1] );
			break;

		case IMVG_KEEPALIVE:
			sprintf(Buffer, "CMD_ID=IMVG_KEEP_ALIVE\r\nTRANSACTION_ID=%s\r\nIMVG_ID=%s\r\nIMVG_VERSION_DATE=IMSPRIME_1.0.2_15_02_2018\r\nSIGNATURE=0x66",TranscationId, argv[1]);
			break;

		case RUN_IMVG:
			printf("\n\n RUN_IMVG Selected \n\n");
			GetTimeStamp (TimeStamp) ;
			TimeStamp[63] = '\0';
			sprintf(Buffer,"CMD_ID=DEVICE_ALERT\r\nTRANSACTION_ID=%s\r\nIMVG_ID=%s\r\nDEVICE_ID=%s\r\nALERT_TYPE=ACTIVE\r\nALERT_EVENT=IMVG_UP\r\nIMVG_TIME_STAMP=%s\r\nUSR_DTLS=MAIN_USR\r\nIMVG_VERSION=IMSPRIME_1.0.2_15_02_2018\r\nSIGNATURE=0x3d66\r\n",TranscationId, argv[1], argv[1], TimeStamp);
			break;
		default:
			printf("Invalid Command\n");
			break;
		}

		strcpy(sendbuf,Buffer);
		len=strlen(Buffer);
		SendMsgToCMS(sendbuf,len);
		ret = SSL_read(ssl, recvbuf, MAX_RECV_BUF_LEN - 1);
		if (ret <= 0) {
			printf("SSL_Read Error; CMS Closed Connection, trying to connect again.\n");
			SSL_free(ssl);
			close(server);
			X509_free(cert);
			SSL_CTX_free(ctx);
			Periodic_KA=0;
			Command = IMVG_UP;
			goto Connect;
		}
		else{
			printf("read %d bytes\n", ret);
			recvbuf[ret+1] = '\0';
			printf("Read Msg: [%s]\n", recvbuf+2);

			if(strstr(recvbuf+2, "DEVICE_ALERT_ACK") > 0){
				Periodic_KA=1;
			}

		}
		printf("Waiting for %d secs..",timeout);
		sleep(timeout);
	}

	/* ---------------------------------------------------------- *
	 * Free the structures we don't need anymore                  *
	 * -----------------------------------------------------------*/
	SSL_free(ssl);
	close(server);
	X509_free(cert);
	SSL_CTX_free(ctx);
	return(0);
}

/* ---------------------------------------------------------- *
 * create_socket() creates the socket & TCP-connect to server *
 * ---------------------------------------------------------- */
int create_socket(char url_str[], BIO *out) {
	int sockfd;
	char hostname[256] = "";
	char    portnum[6] = PORT;
	char      proto[6] = "";
	char      *tmp_ptr = NULL;
	int           port;
	struct hostent *host;
	struct sockaddr_in dest_addr;

	if(strlen(host_addr) == 0){

		/* ---------------------------------------------------------- *
		 * Remove the final / from url_str, if there is one           *
		 * ---------------------------------------------------------- */
		if(url_str[strlen(url_str)] == '/')
			url_str[strlen(url_str)] = '\0';

		/* ---------------------------------------------------------- *
		 * the first : ends the protocol string, i.e. http            *
		 * ---------------------------------------------------------- */
		strncpy(proto, url_str, (strchr(url_str, ':')-url_str));

		/* ---------------------------------------------------------- *
		 * the hostname starts after the "://" part                   *
		 * ---------------------------------------------------------- */
		strncpy(hostname, strstr(url_str, "://")+3, sizeof(hostname));

		/* ---------------------------------------------------------- *
		 * if the hostname contains a colon :, we got a port number   *
		 * ---------------------------------------------------------- */
		if(strchr(hostname, ':')) {
			tmp_ptr = strchr(hostname, ':');
			/* the last : starts the port number, if avail, i.e. 8443 */
			strncpy(portnum, tmp_ptr+1,  sizeof(portnum));
			*tmp_ptr = '\0';
		}

		port = atoi(portnum);

		printf("hostname:%s\n",hostname);
		if ( (host = gethostbyname(hostname)) == NULL ) {
			BIO_printf(out, "Error: Cannot resolve hostname %s.\n",  hostname);
			return -1;
		}
	}
	else{
		port = argport;
		printf("host Addr:%s Port:%d\n",host_addr, argport);
		if ( (host = gethostbyname(host_addr)) == NULL ) {
			BIO_printf(out, "Error: Cannot resolve hostname %s.\n",  host_addr);
			return -1;
		}
	}

	/* ---------------------------------------------------------- *
	 * create the basic TCP socket                                *
	 * ---------------------------------------------------------- */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(port);
	dest_addr.sin_addr.s_addr = *(long*)(host->h_addr);

	/* ---------------------------------------------------------- *
	 * Zeroing the rest of the struct                             *
	 * ---------------------------------------------------------- */
	memset(&(dest_addr.sin_zero), '\0', 8);

	tmp_ptr = inet_ntoa(dest_addr.sin_addr);

	/* ---------------------------------------------------------- *
	 * Try to make the host connect here                          *
	 * ---------------------------------------------------------- */
	if ( connect(sockfd, (struct sockaddr *) &dest_addr,
			sizeof(struct sockaddr)) == -1 ) {
		BIO_printf(out, "Error: Cannot connect to host %s [%s] on port %d.\n",
				hostname, tmp_ptr, port);
		return -1;
	}

	return sockfd;
}
