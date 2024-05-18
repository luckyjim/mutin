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
    void *buffer= NULL;
    errno = 0;
    flags = O_CREAT | O_RDONLY;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;

    //fprintf(stdout, "\nREAD open ");
    mqd = mq_open(MQ_NAME, flags, 0666, &attr);
    return mqd;
}

int sendMessage(Message message, mqd_t mqd)
{
    int flags;

    Message to_send;
    int ret;

    mq_unlink(MQ_NAME);

    to_send = message;
    errno = 0;

    flags = O_CREAT | O_EXCL| O_WRONLY;
    flags = O_CREAT | O_WRONLY;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message);
    attr.mq_curmsgs = 0;

    mqd = mq_open(MQ_NAME, flags, 0600,  &attr );
    //fprintf(stdout, "\n%d %s",errno, strerror(errno));
    //fprintf(stdout, "\n%d",mqd == (mqd_t) -1 );

    if (mqd == (mqd_t) -1)
        errExit("\nmq_open");

    //fprintf(stdout,"\nWrite to MQ : %d", to_send.identifiant);
    //fprintf(stdout,"\nWrite to MQ : %d", to_send.telescope);
    //fprintf(stdout,"\nWrite to MQ : %" PRIi64 "\n", to_send.tps);

    ret = mq_send(mqd, (char *)&message, sizeof(message), 10);
    //fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    //fprintf(stdout, "\nret %d", ret);

    if ( ret == -1)
    {
        fprintf(stdout, "\nret NOK %d", ret);
        errExit("NOK");
    }

    return EXIT_SUCCESS;
}

Message* readMessage(mqd_t mqd)
{
    //Message * to_read;
    //to_read = (Message *) malloc(sizeof(Message));
    static Message to_read;
    int ret;
    unsigned int prio;
    //fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    //fprintf(stdout, "\nmqt %d", mqd);

    if (mqd == (mqd_t) -1){
        fprintf(stdout, "\nopen NOK");
        exit(EXIT_FAILURE);
    }
    //fprintf(stdout, "\nREAD receive");
    //ret = mq_receive(mqd, buffer, 1024, &prio);
    ret = mq_receive(mqd, &to_read, sizeof(Message), &prio);
    //fprintf(stdout, "\nerrno : %d %s",errno, strerror(errno) );
    //fprintf(stdout, "\nret   : %d", ret);
    //fprintf(stdout,"\n%ld", to_read.telescope);
    //fprintf(stdout,"\n%ld", to_read.tps);
    if ( ret == -1){
        fprintf(stdout, "\nmq_receive NOK");
        exit(EXIT_FAILURE);
    }

    return &to_read;
}
