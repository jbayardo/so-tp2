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
        std::size_t lectores;
        std::size_t escritores;
        std::size_t escribiendo;

        pthread_mutex_t mutex;
        pthread_cond_t turno;
};

#endif
