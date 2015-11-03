#ifndef RWLock_h
#define RWLock_h

#include <iostream>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();
        virtual ~RWLock();
    private:
        std::size_t cant_lectores;
        std::size_t cant_escritores;
        
        pthread_mutex_t cant_escritores_m;
        pthread_cond_t hay_escritor_c;

        pthread_mutex_t cant_lectores_m;
        pthread_cond_t hay_lectores_c;
};

#endif
