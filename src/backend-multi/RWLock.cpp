#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock(){
    pthread_mutex_init(&(this->mutex), NULL);
    pthread_cond_init(&(this->turno), NULL);
    lectores = 0;
    escritores = 0;
    escribiendo = 0;
}

RWLock::~RWLock() {
    pthread_cond_destroy(&(this->turno));
    pthread_mutex_destroy(&(this->mutex));
}

void RWLock :: rlock() {
    pthread_mutex_lock(&mutex);

    if (escritores) {
        pthread_cond_wait(&turno, &mutex);
    }

    while (escribiendo) {
        pthread_cond_wait(&turno, &mutex);
    }

    lectores++;
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
    pthread_mutex_lock(&mutex);
    escritores++;

    while (lectores || escribiendo) {
        pthread_cond_wait(&turno, &mutex);
    }

    escribiendo++;
    pthread_mutex_unlock(&mutex);
}

void RWLock :: runlock() {
    pthread_mutex_lock(&mutex);
    lectores--;
    pthread_cond_broadcast(&turno);
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
    pthread_mutex_lock(&mutex);
    escritores--;
    escribiendo--;
    pthread_cond_broadcast(&turno);
    pthread_mutex_unlock(&mutex);
}
