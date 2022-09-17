#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int n, N, p, counter;

int main() {
	// get n as input. there will be N = 2n + 1 philosophers at the table
	printf("> Please enter the value of 'n' (there will be 2n + 1 philosophers at the table): ");
	scanf("%d", &n);
	
	N = 2 * n + 1; // number of philosophers
	int s[N];      // for each philosopher: 0 -> think, 1 -> eat
	
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
		
		// print the virtual table's info
		printf("Virtual table %d members: ", ++counter);
		for (int i=0; i<N; i++)
			if (s[i])
				printf("%d | ", i + 1);
		printf("\b\b  \n----------------------------------------------------\n");
		
		// it takes 1 second for philosophers at the virtual table to finish eating
		sleep(1);
	}
	
	return 0;
}
