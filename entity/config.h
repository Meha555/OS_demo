#ifndef CONFIG_H
#define CONFIG_H

#define _DEBUG

#ifdef _DEBUG
    #include <QDebug>
#endif

#include <QMetaType>
#include <QVariant>

class Config
{
public:
    Config();
    Config(int buf1_size,int buf2_size,int buf3_size,
           int put_n,int put_speed,
           int move_n,int move_speed,
           int get_n,int get_speed);
    //主线程的槽函数调用这2个函数
    void saveConfig(const Config& config);
    Config readConfig() const;
    int getBuffer1_size() const;
    void setBuffer1_size(int value);

    int getBuffer2_size() const;
    void setBuffer2_size(int value);

    int getBuffer3_size() const;
    void setBuffer3_size(int value);

    int getPut_num() const;
    void setPut_num(int value);

    int getPut_speed() const;
    void setPut_speed(int value);

    int getMove_num() const;
    void setMove_num(int value);

    int getMove_speed() const;
    void setMove_speed(int value);

    int getGet_num() const;
    void setGet_num(int value);

    int getGet_speed() const;
    void setGet_speed(int value);

#ifdef _DEBUG
    void configInfo(){
        qDebug()<<"config: "<<c_id<<' '
               <<buffer1_size<<' '
              <<buffer2_size<<' '
             <<buffer3_size<<' '
            <<put_num<<' '
           <<put_speed<<' '
          <<move_num<<' '
         <<move_speed<<' '
        <<get_num <<' '
        <<get_speed;
    }
#endif
private:
    bool flag = false;
public:
    int c_id=0;
    int buffer1_size=0;// buffer1大小
    int buffer2_size=0;// buffer2大小
    int buffer3_size=0;// buffer3大小
    int put_num=0;     // put操作个数
    int put_speed=0;   // put操作速度
    int move_num=0;    // move操作个数
    int move_speed=0;  // move操作速度
    int get_num=0;     // get操作个数
    int get_speed=0;   // get操作速度
};

// 注册入元对象系统，以便QVariant使用
Q_DECLARE_METATYPE(Config)

#endif // CONFIG_H
