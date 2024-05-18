

#include "MQenC.h"

#define MQ_NAME "/GATEMQtoIHM"

void
errExit(const char *format)
{
    fprintf(stderr, "%s",format);
    exit(EXIT_FAILURE);
}


mqd_t init()
{
    int flags;
    mqd_t mqd;
    Message to_send;


    mq_unlink(MQ_NAME);

    //to_send = message;
    errno = 0;
    flags = O_CREAT | O_RDWR;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;

    mqd = mq_open(MQ_NAME, flags, 0600,  &attr );
    //fprintf(stdout, "\n%d %s",errno, strerror(errno));
    //fprintf(stdout, "\n%d",mqd == (mqd_t) -1 );

    if (mqd == (mqd_t) -1)
    {
        errExit("\nmq_open");
    }

    return mqd;
}


int sendMessage(Message message, mqd_t mqd)
{

    return EXIT_SUCCESS;
    int ret;
    //fprintf(stdout,"\nWrite to MQ : %d", to_send.identifiant);
    //fprintf(stdout,"\nWrite to MQ : %d", message.telescope);
    //fprintf(stdout,"\nWrite to MQ : %" PRIi64 "\n", to_send.tps);

    ret = mq_send(mqd, (char *)&message, sizeof(Message), 10);
    //fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    //fprintf(stdout, "\nret %d", ret);

    if ( ret == -1)
    {
        fprintf(stdout, "\nret NOK %d", ret);
        errExit("NOK");
    }

    return EXIT_SUCCESS;
}

int readMessage(mqd_t mqd)
{
    Message to_read;
    int ret;
    unsigned int prio;
    void *buffer= NULL;
    errno = 0;


    buffer = malloc(1024*sizeof(char));
    fprintf(stdout, "\nREAD receive");
    //ret = mq_receive(mqd, buffer, 1024, &prio);
    ret = mq_receive(mqd,(Message *)&to_read, sizeof(Message), &prio);
    fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    fprintf(stdout, "\nret   : %d", ret);
    fprintf(stdout,"\n%ld", to_read.telescope);

    if ( ret == -1){
        fprintf(stdout, "\nmq_receive NOK");
        exit(EXIT_FAILURE);
    }

    return 0;
}
