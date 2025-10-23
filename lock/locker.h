#ifndef LOCKER_H
#define LOCKER_H

#include<pthread.h>

class locker{
private:
    pthread_mutex_t m_mutex;
public:
    locker(){
        pthread_mutex_init(&m_mutex, NULL);
    }
    ~locker(){
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock(){
        return pthread_mutex_lock(&m_mutex) == 0;
    }
    bool unlock(){
        return pthread_mutex_unlock(&m_mutex) == 0;
    }
};





#endif