#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>

int hist[25], samples;

// print the final histogram. the variable "scale" is used as to make sure the printed stars fit in the terminal window
void printHistogram(int scale) {
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
	
	if (argc < 2) { // check if all the required parameters are passed
		printf("Please pass the number of samples as argument.\n");
		return -1;
	}
	samples = atoi(argv[1]);
	
	// record the starting time
	struct timeval begin, end;
	gettimeofday(&begin, 0);
    
	// fill the hist array
	for (int i = 0; i < samples; i++) {
		int counter = 0;
		for (int j = 0; j < 12; j++)
			counter += (rand() % 101 >= 49) ? 1 : -1;
		hist[counter + 12]++;
	}
	
	// compute the elapsed time in milliseconds and print it
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
	long microseconds = end.tv_usec - begin.tv_usec;
	double elapsed = (seconds + microseconds*1e-6) * 1e3;
	printf("\nFinished Processing in %.3f milliseconds.\n", elapsed);
	
	// plot the final result
	printHistogram((samples >= 500) ? samples / 500 : 1);
	
	return 0;
}
