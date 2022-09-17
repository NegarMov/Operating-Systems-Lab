#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *readUtil(void *vargp) {
	// retrieve the socket
	int *temp = (int *)vargp;
	int sock = *temp;
	char buffer[1024];
	
	// read the server messages until an error encounters or the server connection is closed
	while (1) {
		memset(buffer, 0, sizeof(buffer));
		int valread = read(sock, buffer, 1024);
		if (valread < 0) {
			perror("read");
			break;
		}
		else if (valread == 0) {
			printf("Server conncetion closed.\n");
			break;
		}
		if (buffer[0] != '[')
			printf("**NEW MESSAGE**\n\t");
		printf("%s\n", buffer);
	}
}

int main(int argc, char const *argv[]) {
	// check the required arguments
	if (argc < 4) {
		printf("ERR: Please enter [server-host-name], [server-port-number] and [client-name] as arguments correspondingly.\n");
		return -1;
	}
	const char* HOST = argv[1];
	const int   PORT = atoi(argv[2]);
	const char* NAME = argv[3];

	// create a socket
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	// sets all memory cells to zero
	memset(&serv_addr, '0', sizeof(serv_addr));

	// sets port and address family
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, HOST, &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	// connects to the server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	// create a thread to read server messages
	pthread_t read_thread;
	pthread_create(&read_thread, NULL, readUtil, (void *)&sock);

	// send the name to server so it knows who you are
	send(sock, NAME, strlen(NAME), 0);
	while (1) {
		char query[256], command[10];
		fgets(query, 256, stdin);
		query[strlen(query) - 1] = '\0';	
		
		// send the query to server
		send(sock, query, strlen(query), 0);

		// terminate the program if the command was quit
		if (strcmp(query, "quit") == 0) {
			printf("Quiting the program...\n");			
			break;
		}		
	}

	return 0;
}
