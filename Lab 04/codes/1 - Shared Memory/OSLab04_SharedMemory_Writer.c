#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define MEMSIZE 64
#define KEY 1234
int memoryID;
int firstNumber = 8, secondNumber = 17;

int main() {
	// initiate the shared memory using the shared key defined above
	if ((memoryID = shmget(KEY, MEMSIZE * sizeof(int), IPC_CREAT | 0666)) < 0) {
		printf("ERR: Failed to allocate the shared memory.\n");
		return 1;
	}

	// retrieve the shared memory	
	int *memory;
	if ((memory = shmat(memoryID, NULL, 0)) == (int *) -1) {
		printf("ERR: Failed to retrieve the shared memory.\n");
		return 1;
	}
	
	// write the numbers to the shared memory
	memory[0] = firstNumber, memory[1] = secondNumber;
	printf("+ Writer: Just wrote \"%d\" and \"%d\" to the shared memory.\n", firstNumber, secondNumber);
	memory[4] = 1;	
	
	while (!memory[5]);

	// print the reuslt and free the shared memory
	printf("+ Writer: Got result %d from the reader, freeing the shared memory...\n", memory[3]);
	shmctl(memoryID, IPC_RMID, NULL);

	return 0;
}
