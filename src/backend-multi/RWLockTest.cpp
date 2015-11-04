#include "RWLock.h"
#include <pthread.h>
#include <vector>

RWLock l_variable;
int variable = 0;
pthread_mutex_t pantalla;
int NUM_READERS;
int NUM_WRITERS;

void *test_reader(void *parameter) {
	int numero = *((int *)parameter);

	while (1) {
		l_variable.rlock();
		int local(variable);
		l_variable.runlock();

		pthread_mutex_lock(&pantalla);
		std::cout << "[R" << numero << "] Variable: " << local << std::endl;
		pthread_mutex_unlock(&pantalla);

		if (variable == NUM_WRITERS) {
			break;
		}

		usleep(4);
	}

	pthread_exit(NULL);
}

void *test_writer(void *parameter) {
	int numero = *((int *)parameter);

	l_variable.wlock();
	int local(variable);
	++variable;
	l_variable.wunlock();

	pthread_mutex_lock(&pantalla);
	std::cout << "[W" << numero << "] Valor antiguo: " << local << " " << local + 1 << std::endl;
	pthread_mutex_unlock(&pantalla);

	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	NUM_READERS = std::atoi(argv[1]);
	NUM_WRITERS = std::atoi(argv[2]);

	pthread_mutex_init(&pantalla, NULL);
	std::vector<pthread_t> readers(NUM_READERS);
	std::vector<pthread_t> writers(NUM_WRITERS);

	for (int i = 0; i < NUM_READERS; ++i) {
        pthread_t thread;

        if (pthread_create(&thread, NULL, test_reader, (void *)&i)) {
            std::cerr << "Error al crear reader" << std::endl;
            goto error;
        }

        readers[i] = thread;
	}

	for (int i = 0; i < NUM_WRITERS; ++i) {
        pthread_t thread;

        if (pthread_create(&thread, NULL, test_writer, (void *)&i)) {
            std::cerr << "Error al crear writer" << std::endl;
            goto error;
        }

        writers[i] = thread;
	}

	for (int i = 0; i < NUM_READERS; ++i) {
		pthread_join(readers[i], NULL);
	}

	for (int i = 0; i < NUM_WRITERS; ++i) {
		pthread_join(writers[i], NULL);
	}

	pthread_exit(NULL);

error:
	pthread_mutex_destroy(&pantalla);
    return 0;
}

