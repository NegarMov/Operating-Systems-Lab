#include <stdio.h>

struct process {
    int pid; // process ID
    int bt;  // burst time
    int wt;  // waiting time
	int tt;  // turnaround time
};

int n;
float avg_wt, avg_tt;

int main() {
    printf("Number of processes: ");
    scanf("%d", &n);
    struct process processes[n];
	
	printf("\nPlease enter the 'burst time' corresponding to each process:\n");
    for (int i=0; i<n; i++) {
		int bt;
        scanf("%d", &bt);
		processes[i].bt = bt;
		processes[i].pid = i + 1;
    }

    for (int i=0 ; i<n; i++) {
        processes[i].wt = (i == 0)? 0 : processes[i-1].tt;
        avg_wt += processes[i].wt;        
		processes[i].tt = (i == 0)? processes[i].bt : processes[i].wt + processes[i].bt;
		avg_tt += processes[i].tt;
		printf("\nProcess %d:\n", processes[i].pid);
		printf("  Burst Time = %d - Waiting Time = %d - Turnaround Time = %d\n", processes[i].bt, processes[i].wt,  processes[i].tt);
    }
    
    avg_wt /= n, avg_tt /= n;
    printf("\n\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tt);

    return 0;
}
