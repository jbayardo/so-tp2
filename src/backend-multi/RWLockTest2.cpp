#include "RWLock.h"
#include <pthread.h>
#include <cstdlib>
#include <vector>

RWLock l_variable;
int variable = 0;
pthread_mutex_t pantalla;
int NUM_READERS;
int NUM_WRITERS;

void *test_reader(void *parameter) {
	while (1) {
		l_variable.rlock();
		if (variable == NUM_WRITERS) {
			break;
		}		
		l_variable.runlock();
	}
	pthread_exit(NULL);
}

void *test_writer(void *parameter) {
	l_variable.wlock();
	++variable;
	l_variable.wunlock();
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
	std::cerr << "Error" << std::endl;
    return 0;
}

