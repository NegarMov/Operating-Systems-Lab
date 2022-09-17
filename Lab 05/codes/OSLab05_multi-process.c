#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int samples, processes, memoryID, pid = 1;

// print the final histogram. the variable "scale" is used as to make sure the printed stars fit in the terminal window
void printHistogram(int *hist, int scale) {
	printf("\n=====================================");
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < hist[i] / scale; j++)
			printf("*");
		printf("\n");
	}
	printf("=====================================\n");
}

int main(int argc, char *argv[]) {
	srand(time(0));
	
	if (argc < 3) { // check if all the required parameters are passed
		printf("Please pass the number of samples and processes as argument.\n");
		return -1;
	}
	samples = atoi(argv[1]);
	processes = atoi(argv[2]);

	// initiate the shared memory
	memoryID = shmget(IPC_PRIVATE, 25 * sizeof(int), IPC_CREAT | 0666);

	// create child processes
	for (int i = 0; i < processes; i++)
		if (pid > 0) // fork only if we are in the main process
			pid = fork();
		else
			break;

	// retrieve the shared memory	
	int *hist = (int *) shmat(memoryID, NULL, 0);

	if (pid > 0) { // we are in the main process
		// record the starting time
		struct timeval begin, end;
		gettimeofday(&begin, 0);

		// wait for all children to finish their process
		for (int i = 0; i < processes; i++)
			wait(NULL);
		
		// compute the elapsed time in milliseconds and print it
		gettimeofday(&end, 0);
		long seconds = end.tv_sec - begin.tv_sec;
		long microseconds = end.tv_usec - begin.tv_usec;
		double elapsed = (seconds + microseconds*1e-6) * 1e3;
		printf("\nFinished Processing in %.3f milliseconds.\n", elapsed);
			
		// plot the final result
		printHistogram(hist, (samples >= 500) ? samples / 500 : 1);

		// free the shared memory		
		shmctl(memoryID, IPC_RMID, NULL);
	}
	else { // we are in one of the child processes, so we need to fill the hist array
		for (int i = 0; i < samples / processes; i++) {
			int counter = 0;
			for (int j = 0; j < 12; j++)
				counter += (rand() % 101 >= 49) ? 1 : -1;
			*(hist + counter + 12) = *(hist + counter + 12) + 1;
		}
	}
	
	return 0;
}
