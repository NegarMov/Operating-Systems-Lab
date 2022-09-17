#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

# define MAX 100
pthread_t philosophers[MAX];
pthread_mutex_t chopstick[MAX];
int s[MAX]; // for each philosopher: 0 -> think, 1 -> eat
int n, N, p;

void *thinkAndEat(int n) {
	while (1) {
		printf("philosopher %d is thinking!!\n", n + 1);

		while (!s[n]);

		// acquire the chopsticks
		pthread_mutex_lock(&chopstick[n]);
		pthread_mutex_lock(&chopstick[(n + 1) % N]);

		printf("philosopher %d is eating using chopstick[%d] and chopstick[%d]!!\n", n + 1, n, (n + 1) % N);
		while (s[n]);

		printf("philosopher %d finished eating!!\n", n + 1);

		// release the chopsticks
		pthread_mutex_unlock(&chopstick[n]);
		pthread_mutex_unlock(&chopstick[(n + 1) % N]);
	}
}

int main() {
	// get n as input. there will be N = 2n + 1 philosophers at the table
	printf("> Please enter the value of 'n' (there will be 2n + 1 philosophers at the table): ");
	scanf("%d", &n);
	N = 2 * n + 1; // number of philosophers

	// initiate the mutex locks used for accessing the chopsticks
	for (int i=0; i<N; i++)
		pthread_mutex_init(&chopstick[i], NULL);
	
	// create the threads representing the philosophers
	for (int i=0; i<N; i++)
		pthread_create(&philosophers[i], NULL, (void *) thinkAndEat, (void *)(intptr_t) i);

	while (1) { // decide which philasophors can eat at the same time in each round
		// fill the s array, which indicates which philosophers should be eating and which philosophers should be thinking
		memset(s, 0, sizeof s);
		int curr = p;
		if (N == 1)
			s[0] = 1;
		else
			while (curr != ((p - 1) + N) % N)
				s[curr] = 1, curr = (curr + 2) % N;
		p = (p + 1) % N;
		
		// we give 1 second to the philosophers at the virtual table to finish eating
		sleep(1);
	}

	return 0;
}
