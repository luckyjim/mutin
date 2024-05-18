#ifndef MQENC_H_
#define MQENC_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>            /* Defines O_* constants */
#include <sys/stat.h>         /* Defines mode constants */
#include <mqueue.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include <stdint.h>

typedef struct Message
{
    int8_t identifiant;
    int64_t data;
    int8_t telescope;
} Message;

#ifdef __cplusplus
extern "C" {
#endif

mqd_t init();
int sendMessage(Message message, mqd_t mqd);
Message *readMessage(mqd_t mqd);

#ifdef __cplusplus
}
#endif

#endif
