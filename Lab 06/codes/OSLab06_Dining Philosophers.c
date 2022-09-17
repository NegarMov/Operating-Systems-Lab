#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_t philosophers[5];
pthread_mutex_t chopstick[5];

void *thinkAndEat(int n) {
	printf("philosopher %d is thinking!!\n", n + 1);

	// acquire the chopsticks
	pthread_mutex_lock(&chopstick[n]);
	pthread_mutex_lock(&chopstick[(n + 1) % 5]);

	// eat (it takes 1 second)
	printf("philosopher %d is eating using chopstick[%d] and chopstick[%d]!!\n", n + 1, n, (n + 1) % 5);
	sleep(1);

	// release the chopsticks
	pthread_mutex_unlock(&chopstick[n]);
	pthread_mutex_unlock(&chopstick[(n + 1) % 5]);

	printf("philosopher %d finished eating!!\n", n + 1);
}

int main() {
	// initiate the mutex locks used for accessing the chopsticks
	for (int i=0; i<5; i++)
		pthread_mutex_init(&chopstick[i], NULL);

	// create the threads representing the philosophers
	for (int i=0; i<5; i++)
		pthread_create(&philosophers[i], NULL, (void *) thinkAndEat, (void *)(intptr_t) i);

	// wait for all the threads to finish
	for (int i=0; i<5; i++)
		pthread_join(philosophers[i], NULL);

	return 0;
}
