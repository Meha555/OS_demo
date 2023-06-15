#ifndef UTILS_H
#define UTILS_H

// 不知道为什么只能用typedef，用不了using
//using ThreadState = enum{RUNNING,BLOCK,TERMINATED};
typedef enum{RUNNING,BLOCK,TERMINATED} ThreadState;

typedef struct{
    int put_blocked_num; //当前阻塞的put线程数
    int move_blocked_num; //当前阻塞的move线程数
    int get_blocked_num; //当前阻塞的get线程数
} Statitics;

#endif // UTILS_H
