#ifndef MESSAGEMQ_H
#define MESSAGEMQ_H

#include "MQenC.h"
#include <cinttypes>

class MessageMQ
{
public:
    MessageMQ();

    void init(Message * message);

    int8_t name();
    void setName(int8_t name);

    int8_t type();
    void setType(int8_t type);

    int64_t data();
    void setData(int64_t data);

private:
    int8_t m_name;
    int8_t m_type;
    int64_t m_data;
};

#endif // MESSAGEMQ_H
