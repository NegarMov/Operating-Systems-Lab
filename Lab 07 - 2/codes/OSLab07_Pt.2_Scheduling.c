#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>

struct job {
	int arrival_time;
	int burst_time;
	int sliceable; // 0 -> cannot be sliced, 1 -> can be sliced
};

#define MAX_PROCESS_COUNT 20

int n;
struct job jobs[MAX_PROCESS_COUNT];

// swap two jobs by reference (used in sort algorithm)
void swap(struct job* a, struct job* b) {
    struct job tmp = *a;
    *a = *b, *b = tmp;
}
 
// sort processes according to their arriving time
void sort() {
    for (int i=0; i<n-1; i++)
        for (int j=0; j<n-i-1; j++)
            if (jobs[j].arrival_time > jobs[j+1].arrival_time)
                swap(&jobs[j], &jobs[j+1]);
}

int main() {
	printf("Number of processes: ");
	scanf("%d", &n);
	
	printf("\nPlease enter 'arrival time' and 'burst time' corresponding to each process\nand determine if it 'can be sliced (1) or not (0)':\n");
	for (int i=0; i<n; i++)
		scanf("%d%d%d", &jobs[i].arrival_time, &jobs[i].burst_time, &jobs[i].sliceable);
		
	sort();
	
	printf("\nSchedule:\n");
	int time = 0, start_time = 0, curr = 0;
	bool running = false;
	while (curr < n) {
		if (!running && jobs[curr].arrival_time <= time) { // select a new eligible job to run
			running = true;
			start_time = time;
		}
		if (running && time - start_time == jobs[curr].burst_time) {
			int tat = time - jobs[curr].arrival_time;
			int wt = start_time - jobs[curr].arrival_time;
			printf("%d -> %d : Process %d (turnaround time = %d, waiting time = %d)\n", start_time, time, ++curr, tat, wt);
			running = false;
			continue;
		}
		time++;
	}
	
	return 0;
}
