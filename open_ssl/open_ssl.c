#include <stddef.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define OPENSSL_DEMO_THREAD_NAME "ssl_demo"
#define OPENSSL_DEMO_THREAD_STACK_WORDS 2048
#define OPENSSL_DEMO_THREAD_PRORIOTY 6

#define OPENSSL_DEMO_FRAGMENT_SIZE 8192
//#define OPENSSL_DEMO_FRAGMENT_SIZE 150

#define OPENSSL_DEMO_LOCAL_TCP_PORT 1000

//#define OPENSSL_DEMO_TARGET_NAME "www.baidu.com"
//#define OPENSSL_DEMO_TARGET_TCP_PORT 443
#define OPENSSL_DEMO_TARGET_NAME "10.0.0.15"
#define OPENSSL_DEMO_TARGET_TCP_PORT 4900

//#define OPENSSL_DEMO_REQUEST "{\"path\": \"/v1/ping/\", \"method\": \"GET\"}\r\n"
#define OPENSSL_DEMO_REQUEST "CMD_ID=IMVG_REGISTER\r\nTRANSACTION_ID=625558ec\r\nIMVG_ID=00:0e:8f:65:78:8a\r\nSIGNATURE=0x1360\r\n"

#define OPENSSL_DEMO_RECV_BUF_LEN 1024
//#define OPENSSL_DEMO_RECV_BUF_LEN 150

//LOCAL xTaskHandle openssl_handle;

char send_data[] = OPENSSL_DEMO_REQUEST;
int send_bytes = sizeof(send_data);

char recv_buf[OPENSSL_DEMO_RECV_BUF_LEN];

int main()
{
    int ret,err,Index;
    unsigned short	BSize;

    SSL_CTX *ctx;
    SSL *ssl;

    int sock;
    struct sockaddr_in sock_addr;

    ip_addr_t target_ip;

    int recv_bytes = 0;

    printf("OpenSSL demo thread start...\n");


    printf("create SSL context ......");
    //ctx = SSL_CTX_new(TLSv1_1_client_method());
    ctx = SSL_CTX_new(TLSv1_client_method());
    if (!ctx) {
        printf("failed\n");
        goto failed1;
    }
    printf("OK\n");

    printf("set SSL context read buffer size ......");
    SSL_CTX_set_default_read_buffer_len(ctx, OPENSSL_DEMO_FRAGMENT_SIZE);
    ret = 0;
    if (ret) {
        printf("failed, return %d\n", ret);
        goto failed2;
    }
    printf("OK\n");

    printf("create socket ......");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        printf("failed\n");
        goto failed3;
    }
    printf("OK\n");

    printf("bind socket ......");
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = 0;
    sock_addr.sin_port = htons(OPENSSL_DEMO_LOCAL_TCP_PORT);
    ret = bind(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
    if (ret) {
        printf("failed\n");
        goto failed4;
    }
    printf("OK\n");

    printf("socket connect to remote ......");
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(OPENSSL_DEMO_TARGET_NAME);
    sock_addr.sin_port = htons(OPENSSL_DEMO_TARGET_TCP_PORT);
    ret = connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
    if (ret) {
        printf("failed\n", OPENSSL_DEMO_TARGET_NAME);
        goto failed5;
    }
    printf("OK\n");

    printf("create SSL ......");
    ssl = SSL_new(ctx);
    if (!ssl) {
        printf("failed\n");
        goto failed6;
    }
    printf("OK\n");

    SSL_set_fd(ssl, sock);

    printf("SSL connected to %s port %d ......", OPENSSL_DEMO_TARGET_NAME, OPENSSL_DEMO_TARGET_TCP_PORT);
    ret = SSL_connect(ssl);
    if (!ret) {
        printf("failed, return [-0x%x]\n", -ret);
	err=SSL_get_error(ssl, ret);
	printf("SSL ERROR: %d\n",err);
        goto failed7;
    }
    printf("OK\n");

    BSize = htons (send_bytes) ;
    for (Index = send_bytes + 1; Index >= 2; Index--)
	send_data [Index] = send_data [Index - 2] ;
    memcpy (send_data, &BSize, 2) ;
    send_bytes += 2 ;

    printf("send request to %s port %d ......", OPENSSL_DEMO_TARGET_NAME, OPENSSL_DEMO_TARGET_TCP_PORT);
    ret = SSL_write(ssl, send_data, send_bytes);
    if (ret <= 0) {
        printf("failed, return [-0x%x]\n", -ret);
        goto failed8;


    }
    printf("send data: %s of size : %d AND SSL Write returns:%d......", send_data, send_bytes, ret);
    printf("OK\n\n");
    do {
        ret = SSL_read(ssl, recv_buf, OPENSSL_DEMO_RECV_BUF_LEN - 1);
        if (ret <= 0) {
            break;
        }
        recv_bytes += ret;
        printf("%s", recv_buf);
    } while (1);
    printf("read %d bytes data from %s ......\n", recv_bytes, OPENSSL_DEMO_TARGET_NAME);

failed8:
    SSL_shutdown(ssl);
failed7:
    SSL_free(ssl);
failed6:
failed5:
failed4:
    close(sock);
failed3:
failed2:
    SSL_CTX_free(ctx);
failed1:
    vTaskDelete(NULL);

    printf("task exit\n");

    return 0;
}
