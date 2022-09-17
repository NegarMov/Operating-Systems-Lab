#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUMBER_OF_RESOURCES 5
#define NUMBER_OF_CUSTOMERS 5

pthread_mutex_t lock;

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = { 0 };
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

void printRequest(int request[]) {
    // print the request array in a format like [a, b, ..., z]
	printf("[");
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		printf("%d, ", request[i]);
	printf("\b\b]\n");
}

bool checkSafety() {
    // initiate the finish and work tables
	bool finish[NUMBER_OF_CUSTOMERS] = { 0 };
    int work[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];
    
    // do it until finish = true for every customer
	int count = 0;
    while (count != NUMBER_OF_CUSTOMERS) {
        bool changed = false;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        	bool flag = true;
        	for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        		if (need[i][j] > work[i])
        			flag = false;

            // both of the conditions should hold, if so, mark it as finished
            if (!finish[i] && flag) {
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    work[j] += allocation[i][j];
                finish[i] = true, changed = true;
                count++;
            }
        }
        if (!changed)
            // there exist at least one process which we may not be able to grant its upcoming request
            return false;
    }

    return true;
}

int request_resources(int customer_num, int request[]) {
    // if the process has a request which will exceed its need, it cannot be accepted
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        if (request[i] > need[customer_num][i] || request[i] > available[i])
            return -1;

    // acquire the lock
    pthread_mutex_lock(&lock);

    // pretend the request is accepted and update the tables
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // if we will be in a safe state after accepting the request, we will accept it
    if (checkSafety()) {
        printf("[ACCEPTED] - Customer number %d - ", customer_num);
	    printRequest(request);
        pthread_mutex_unlock(&lock);
        return 0;
    }
    // else, we have to decline it
	printf("[DECLINED] - Customer number %d - ", customer_num);
    printRequest(request);
	
    // the request has been declined, so rollback
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }
    
    // release the lock
    pthread_mutex_unlock(&lock);
    
    return -1;
}

int release_resources(int customer_num, int request[]) {
    // acquire the lock
    pthread_mutex_lock(&lock);

    // log info
    printf("[RELEASED] - Customer number %d - ", customer_num);
    printRequest(request);

    // update the tables; add to the available and needed resources, decrease the allocated resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }

    // release the lock
    pthread_mutex_unlock(&lock);
}

void *runCustomer(int n) {
    int requst[NUMBER_OF_RESOURCES];
    
    while (1) {
        if (rand() % 2 == 1) { // request resources
            // fill your request with random numbers limited to need[n]
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                requst[j] = rand() % (need[n][j] + 1);
            request_resources(n, requst);  
        } 
		else { // release resources
            // fill your request with random numbers limited to allocation[n]
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                requst[j] = rand() % (allocation[n][j] + 1);
            release_resources(n, requst);
        }

        // wait for a random amount of time
        sleep(rand() % 3);
    }
}

int main(int argc, char const *argv[]) {
    // set the seed for random generator
    srand(time(NULL));

    // check the number of arguments, these arguments represent the number of available resources
    if (argc < NUMBER_OF_RESOURCES + 1) {
        printf("Not enough arguments\n");
        return EXIT_FAILURE;
    }

    // fill the available array according to the given arguments
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		available[i] = strtol(argv[i + 1], NULL, 10);

    // fill the max array according to the values in max.txt file
    FILE *f_ptr = fopen("max.txt", "r");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            fscanf(f_ptr, "%d", &maximum[i][j]);
            need[i][j] = maximum[i][j];
        }

    // initiate the mutex lock used to protect access to our shared arrays
    pthread_mutex_init(&lock, NULL);

    // initiate the threads, each thread represents a customer
    pthread_t customerThreads[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        pthread_create(&customerThreads[i], NULL, (void *) runCustomer, (void *)(intptr_t) i);
    
    // wait for the threads to finish their jobs
    for (int i = 0; i < 5; i++)
        pthread_join(customerThreads[i], NULL);

    return 0;
}
