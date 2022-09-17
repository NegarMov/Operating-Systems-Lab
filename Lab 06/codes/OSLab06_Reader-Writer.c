#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

// student ID: 9831062
# define LIMIT 2
int memoryID, pid = 1;

int main(int argc, char *argv[]) {
	// initiate the shared memory
	memoryID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

	// create child processes
	pid = fork() && fork();

	// retrieve the shared memory	
	int* counter = (int *) shmat(memoryID, NULL, 0);

	if (pid > 0) { // we are in the main (Writer) process
		while (*counter < LIMIT) {
			// increment the counter
			*counter = *counter + 1;
			printf("%d: updated counter to %d\n", getpid(), *counter);
		}

		// wait for the Readers to finish
		wait(NULL); wait(NULL);

		// free the shared memory		
		shmctl(memoryID, IPC_RMID, NULL);
	}
	else { // we are in one of the child (Reader) processes
		int tmp;
		while ((tmp = *counter) < LIMIT)
			printf("%d: read %d from counter\n", getpid(), tmp);
		printf("%d: read %d from counter\n", getpid(), tmp);
	}
	
	return 0;
}
