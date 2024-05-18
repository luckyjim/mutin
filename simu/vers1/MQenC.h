#ifndef MQENC_H_
#define MQENC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>            /* Defines O_* constants */
#include <sys/stat.h>         /* Defines mode constants */
#include <mqueue.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Message
{
    int8_t identifiant;
    // data description follow ID
    // 0: evenement (bruit ou gerbe)
    // 1: clock offset
    // 2: HT
    // 3: rate trigger (TO DO)
    int64_t data;
    int8_t telescope;
} Message;

mqd_t init();
int sendMessage(Message message, mqd_t mqd);
int readMessages();

#ifdef __cplusplus
}
#endif

#endif
