#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>            /* Defines O_* constants */
#include <sys/stat.h>         /* Defines mode constants */
#include <mqueue.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#define MQ_NAME "/jmcMQ"



typedef struct DataTeles
{
  long id;
  char data[512];
} DataTeles;



void
errExit(const char *format)
{

    fprintf(stderr, "%s",format);
    exit(EXIT_FAILURE);
}


int sendMessageExample(char send[])
{
    int flags;
    mqd_t mqd;
    DataTeles myStruct;
    int ret;

    mq_unlink(MQ_NAME);

    myStruct.id= 314;
    strcpy(myStruct.data, send);
    errno = 0;

    flags = O_CREAT | O_EXCL| O_WRONLY;
    flags = O_CREAT | O_WRONLY;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(DataTeles);
    attr.mq_curmsgs = 0;

    mqd = mq_open(MQ_NAME, flags, 0600,  &attr );
    fprintf(stdout, "\n%d %s",errno, strerror(errno));
    fprintf(stdout, "\n%d",mqd == (mqd_t) -1 );

    if (mqd == (mqd_t) -1)
        errExit("\nmq_open");

    ret = mq_send(mqd, (char *)&myStruct, sizeof(DataTeles), 10);
    fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    fprintf(stdout, "\nret %d", ret);

    if ( ret == -1)
    {
        fprintf(stdout, "\nret NOK %d", ret);
        errExit("NOK");
    }

    return EXIT_SUCCESS;
}

int readMessageExample()
{
    int flags;
    mqd_t mqd;
    DataTeles myStruct;
    int ret;
    unsigned int prio;
    void *buffer= NULL;
    errno = 0;
    flags = O_RDONLY;
    myStruct.id= 0;
    strcpy(myStruct.data, "rien");

    fprintf(stdout, "\nREAD open ");
    mqd = mq_open(MQ_NAME, flags);
    fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    fprintf(stdout, "\nmqt %d", mqd);

    if (mqd == (mqd_t) -1){
        fprintf(stdout, "\nopen NOK");
        exit(EXIT_FAILURE);
    }

    buffer = malloc(1024*sizeof(char));
    fprintf(stdout, "\nREAD receive");
    //ret = mq_receive(mqd, buffer, 1024, &prio);
    ret = mq_receive(mqd,(char *)&myStruct, sizeof(DataTeles), &prio);
    fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    fprintf(stdout, "\nret   : %d", ret);
    fprintf(stdout,"\n%ld", myStruct.id);
    fprintf(stdout,"\n%s", myStruct.data);

    if ( ret == -1){
        fprintf(stdout, "\nmq_receive NOK");
        exit(EXIT_FAILURE);
    }

    return 0;
}
