#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

#include "common.h"


int main(int argc, char **argv)
{
    mqd_t mq;
    char buffer[MAX_SIZE];
    MSG msg;

	msg.command=25;
	msg.num=500;
	strcpy(msg.buf,"test Q!!");

    /* open the mail queue */
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    CHECK((mqd_t)-1 != mq);


    printf("Send to server (enter \"exit\" to stop it):\n");

    do {
        printf("> ");
        fflush(stdout);

        memset(buffer, 0, MAX_SIZE);
        fgets(buffer, MAX_SIZE, stdin);

        /* send the message */
        //CHECK(0 <= mq_send(mq, buffer, MAX_SIZE, 0));
	printf("size of: %lu\n",  sizeof(msg));
        CHECK(0 <= mq_send(mq, (char*)&msg, sizeof(msg), 0));

    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));

    /* cleanup */
    CHECK((mqd_t)-1 != mq_close(mq));

    return 0;
}
