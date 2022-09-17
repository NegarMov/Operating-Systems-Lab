#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define MEMSIZE 64
#define KEY 1234
int memoryID;

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

	// get the numbers from the shared memory
	while (!memory[4]);
	printf("- Reader: Got numbers \"%d\" and \"%d\" from the shared memory.\n", memory[0], memory[1]);
	
	// write the result to the shared memory
	int sum = memory[0] + memory[1];
	memory[3] = sum, memory[5] = 1;
	printf("- Reader: Just wrote the sum result \"%d\" to the shared memory.\n", sum);

	return 0;
}
