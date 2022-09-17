#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

void toggleCase(char *str) {
	for (int i = 0; str[i] != '\0'; i++)
		str[i] += (str[i] >= 'a' && str[i] <= 'z') ? -32 : (str[i] >= 'A' && str[i] <= 'Z') ? 32 : 0;
	
}

#define MSGCNT 4
int mainPipe[2], toggledPipe[2]; // 1 -> write, 0 -> read
pid_t pid;

int main() {
	// initiate pipelines
	if (pipe(mainPipe) < 0 || pipe(toggledPipe) < 0) {
		printf("ERR: Failed to initiate pipes.\n");
		return 1;	
	}

	// create the child process
	if ((pid = fork()) < 0) {
		printf("ERR: Failed to fork child process.\n");
		return 1;
	}

	if (pid > 0) {
		// parent process will only write to the "main pipe" and read from the "toggled pipe"
		close(mainPipe[0]);
		close(toggledPipe[1]);

		char* messages[MSGCNT] = {
			"HellO!",
			"This IS a mEssage",
			"from Parent PROCESS to CHILD process",
			"hope You're doing ALright"
		};

		for (int i = 0; i < MSGCNT; i++) {
			printf("+ Sending: %s\n", messages[i]);
			write(mainPipe[1], messages[i], strlen(messages[i]) + 1);

			char buff[128];
			int bytes = read(toggledPipe[0], buff, sizeof(buff));
			printf("- Received: %s\n", buff);
		}
	}
	else {
		// child process will only read from the "main pipe" and write to the "toggled pipe"
		close(mainPipe[1]);
		close(toggledPipe[0]);

		for (int i = 0; i < MSGCNT; i++) {
			char buff[128];
			int bytes = read(mainPipe[0], buff, sizeof(buff));

			toggleCase(buff);
			write(toggledPipe[1], buff, strlen(buff) + 1);
		}
	}

	return 0;
}

