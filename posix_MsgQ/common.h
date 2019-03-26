#ifndef COMMON_H_
#define COMMON_H_

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1500
#define MSG_STOP    "exit"

typedef struct{
	char command;
	int  num;
	char buf[1024];		
}MSG;

/*typedef struct{
	char command;
	int  num;
	char buf[1000];		
}MSG1;*/

#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \


#endif /* #ifndef COMMON_H_ */
