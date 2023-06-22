#include "message.h"

Message::Message()
{
//    m_id++;
}

Message::Message(int tid, int bid, Op_Type type,const QString &data)
    :t_id(tid),b_id(bid),op_type(type),data(data)
{
//    m_id++;
}

#ifdef _DEBUG
#include <QDebug>
void Message::messageInfo(){
    qDebug()<<"message: "<<m_id<<' '
           <<t_id<<' '
          <<b_id<<' '
         <<op_type<<' '
        <<data;
}
#endif
