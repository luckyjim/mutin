#include "messagemq.h"

MessageMQ::MessageMQ()
{
    m_name = 0;
    m_type = 0;
    m_data = 0;
}

void MessageMQ::init(Message *message)
{
    m_name = message->telescope;
    m_type = message->identifiant;
    m_data = message->data;
}

int8_t MessageMQ::name()
{
    return m_name;
}

void MessageMQ::setName(int8_t name)
{
    m_name = name;
}
int8_t MessageMQ::type()
{
    return m_type;
}

void MessageMQ::setType(int8_t type)
{
    m_type = type;
}
int64_t MessageMQ::data()
{
    return m_data;
}

void MessageMQ::setData(int64_t data)
{
    m_data = data;
}
