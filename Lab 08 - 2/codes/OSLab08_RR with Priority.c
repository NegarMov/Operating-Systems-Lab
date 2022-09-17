#include <stdio.h>

struct process {
    int pid; // process ID
    int bt;  // burst time
    int wt;  // waiting time
	int tt;  // turnaround time
	int pri; // process priority
};

int n, q;
float avg_wt, avg_tt;

int main() {
    printf("Number of processes: ");
    scanf("%d", &n);
    struct process processes[n];
    int remainingJob[n];
    
    printf("Base Time quantum: ");
    scanf("%d", &q);
	
	printf("\nPlease enter the 'burst time' and 'priority' corresponding to each process:\n");
    for (int i=0; i<n; i++) {
		int bt, pri;
        scanf("%d %d", &bt, &pri);
        remainingJob[i] = bt;
		processes[i].bt = bt;
		processes[i].pri = pri;
		processes[i].pid = i + 1;
    }
    printf("\n");

	int flag = 1, curr = 0, time = 0;
	while (flag) {
		flag = 0;
		for (int i=0; i<n; i++) {
			int index = (i + curr) % n, quantum = q / processes[index].pri;
			if (remainingJob[index] > 0) {
				flag = 1;
				if (processes[index].pri == 1) { // NMI
					printf("> Process %d runs for %d time unit(s).\n", processes[index].pid, remainingJob[index]);
					time += remainingJob[index];
					remainingJob[index] = 0;
				}
				else {
					if (remainingJob[index] >= quantum) {
						printf("> Process %d runs for %d time unit(s).\n", processes[index].pid, quantum);
						time += quantum;
						remainingJob[index] -= quantum;
					}
					else {
						printf("> Process %d runs for %d time unit(s).\n", processes[index].pid, remainingJob[index]);
						time += remainingJob[index];
						remainingJob[index] = 0;
					}
				}
				if (remainingJob[index] == 0) {
					avg_wt += (processes[index].wt = time - processes[index].bt);
					avg_tt += (processes[index].tt = time);
				}
				curr++;
				break;
			}
		}
	}
	
	for (int i=0 ; i<n; i++) {
        printf("\nProcess %d:\n", processes[i].pid);
		printf("  Burst Time = %d - Waiting Time = %d - Turnaround Time = %d\n", processes[i].bt, processes[i].wt,  processes[i].tt);
    }
    
    avg_wt /= n, avg_tt /= n;
    printf("\n\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tt);

    return 0;
}
