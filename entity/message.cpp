#include "message.h"

int Message::m_id = 1;

Message::Message()
{
    m_id++;
}

Message::Message(int tid, int bid, int dataid, QString type, QString data)
    :t_id(tid),b_id(bid),data_id(dataid),op_type(type),data(data)
{
    m_id++;
}
