#include "message.h"

int Message::m_id = 0;

Message::Message()
{
    m_id++;
}

Message::Message(int tid, int bid, Op_Type type,const QString &data)
    :t_id(tid),b_id(bid),op_type(type),data(data)
{
    m_id++;
}
