#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() : cant_lectores(0), cant_escritores(0) {
    pthread_mutex_init(&(this->cant_escritores_m), NULL);
    pthread_cond_init(&(this->hay_escritor_c), NULL);

    pthread_mutex_init(&(this->cant_lectores_m), NULL);
    pthread_cond_init(&(this->hay_lectores_c), NULL);
}

RWLock::~RWLock() {
    pthread_cond_destroy(&(this->hay_escritor_c));
    pthread_mutex_destroy(&(this->cant_escritores_m));

    pthread_cond_destroy(&(this->hay_lectores_c));
    pthread_mutex_destroy(&(this->cant_lectores_m));
}

void RWLock :: rlock() {
    pthread_mutex_lock(&this->cant_escritores_m);

    while (this->cant_escritores > 0) {
        pthread_cond_wait(&this->hay_escritor_c, &this->cant_escritores_m);
    }

    pthread_mutex_lock(&this->cant_lectores_m);
    this->cant_lectores++;
    pthread_mutex_unlock(&this->cant_lectores_m);

    pthread_mutex_unlock(&this->cant_escritores_m);
}

void RWLock :: wlock() {
    pthread_mutex_lock(&this->cant_lectores_m);

    while (this->cant_lectores > 0) {
        pthread_cond_wait(&this->hay_lectores_c, &this->cant_lectores_m);
    }

    pthread_mutex_lock(&this->cant_escritores_m);

    while (this->cant_escritores > 0) {
        pthread_cond_wait(&this->hay_escritor_c, &this->cant_escritores_m);
    }

    this->cant_escritores++;

    pthread_mutex_unlock(&this->cant_escritores_m);
    pthread_mutex_unlock(&this->cant_lectores_m);
}

void RWLock :: runlock() {
    pthread_mutex_lock(&this->cant_lectores_m);
    this->cant_lectores--;

    if (this->cant_lectores == 0) {
        pthread_cond_signal(&this->hay_lectores_c);
    }

    pthread_mutex_unlock(&this->cant_lectores_m);
}

void RWLock :: wunlock() {
    pthread_mutex_lock(&this->cant_escritores_m);
    this->cant_escritores--;

    pthread_cond_broadcast(&this->hay_escritor_c);
    pthread_mutex_unlock(&this->cant_escritores_m);
}
