#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "genetic_algorithm.h"

int main(int argc, char *argv[]) {
	pthread_barrier_t *barrier = malloc(sizeof(pthread_barrier_t));
	int id;
	void *status;
    int NUM_THREADS = atoi(argv[3]);
	pthread_t threads[NUM_THREADS];
	struct ThreadArguments *arguments = malloc(NUM_THREADS * sizeof(struct ThreadArguments));

	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, argc, argv)) {
		return 0;
	}

	// Allocate memory for current and next generation for cmp function
	individual *current_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_count, sizeof(individual));
	
	// Initialize barrier
	pthread_barrier_init(barrier, NULL, NUM_THREADS);

	// Create threads
	for (id = 0; id < NUM_THREADS; id++) {
		// Set values for thread arguments
		arguments[id].thread_id = id;
		arguments[id].barrier = barrier;
		arguments[id].num_threads = NUM_THREADS;
		arguments[id].objects = objects;
		arguments[id].object_count = object_count;
		arguments[id].sack_capacity = sack_capacity;
		arguments[id].generations_count = generations_count;
		arguments[id].current_generation = current_generation;
		arguments[id].next_generation = next_generation;

        pthread_create(&threads[id], NULL, (void*)run_genetic_algorithm, &arguments[id]);
    }

	// Join threads
	for (id = 0; id < NUM_THREADS; id++) {
		pthread_join(threads[id], &status);
	}

	// Destroy barrier
	pthread_barrier_destroy(barrier);

	// free resources
	free(current_generation);
	free(next_generation);
	free(objects);
	free(arguments);
	pthread_exit(NULL);

	return 0;
}
