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

int8_t MessageMQ::name() const
{
    return m_name;
}

void MessageMQ::setName(const int8_t &name)
{
    m_name = name;
}
int8_t MessageMQ::type() const
{
    return m_type;
}

void MessageMQ::setType(const int8_t &type)
{
    m_type = type;
}
int64_t MessageMQ::data() const
{
    return m_data;
}

void MessageMQ::setData(const int64_t &data)
{
    m_data = data;
}




