#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXGROUPS 50 // maximum number of groups
#define MAXUSERS  50 // maximum number of users in each group

struct group {
    int  groupId;
    int  sockets[MAXUSERS];   // clients socket ids
    char names[MAXUSERS][64]; // clients names
    int  empty[MAXUSERS];     // indicates if each element of the above arrays is available or not
};

struct group groups[MAXGROUPS];

int join(int groupId, int socketId, char name[64]) {
	if (groupId >= 0 && groupId < MAXGROUPS) {
		for (int i = 0; i < MAXUSERS; i++) // check if the user is already a member of this group
			if (groups[groupId].sockets[i] == socketId) {
				printf("[WARN] %s is already a member of group %d.\n", name, groupId + 1);
				return 0;
			}

		for (int i = 0; i < MAXUSERS; i++)  // find an empty index to insert the user's data at
			if (groups[groupId].empty[i] == 1) {
				groups[groupId].empty[i] = 0;
				groups[groupId].sockets[i] = socketId;
				strcpy(groups[groupId].names[i], name);
				printf("[INFO] Added %s to group %d.\n", name, groupId + 1);
				return 0;
			}
	}
	return -1;
}

void leave(int groupId, int socketId, char name[64]) {
	if (groupId >= 0 && groupId < MAXGROUPS)
		for (int i = 0; i < MAXUSERS; i++) // check all the group members, find the user and remove its data
			if (groups[groupId].sockets[i] == socketId) {
				groups[groupId].sockets[i] = -1;
				groups[groupId].empty[i] = 1;
				printf("[INFO] %s left group %d.\n", name, groupId + 1);
				return;
			}
}

int sendMessage(int groupId, int socketId, char name[64], char message[128]) {
	if (groupId >= 0 && groupId < MAXGROUPS) {
		int flag = 0;
		for (int i = 0; i < MAXUSERS; i++) // check if the user is a member of the group (therefore permitted to send a message)
			if (groups[groupId].empty[i] == 0 && groups[groupId].sockets[i] == socketId)
				flag = 1;
		if (flag == 0) {
			printf("[ERR] %s is not a member of group %d, hence can't send a message here.\n", name, groupId + 1);
			return -1;
		}

		// put the new message in format (GROUP [groupId]) - [client-name] : [message]
		char newMessage[128];
		sprintf(newMessage, "(GROUP %d) - %s : %s\n", groupId + 1, name, message);

		// send the message to all the users in the group (except the sender himself)
		printf("[INFO] %s is sending message \"%s\" to group %d.\n", name, message, groupId + 1);
		for (int i = 0; i < MAXUSERS; i++)
		    if (groups[groupId].empty[i] == 0 && groups[groupId].sockets[i] != socketId) {
			send(groups[groupId].sockets[i], newMessage, 128, 0);
			printf("[info] (GROUP %d) Message sent from %s to %s\n", groupId + 1, name, groups[groupId].names[i]);
		    }
		return 0;
	}
}

void *clientHandler(void *vargp) {
	// retrieve the socket
	int *temp = (int *)vargp;
	int sock = *temp;

	// read the client's name
	char buffer[1024] = {0};
	char name[64];
	int valread = read(sock, buffer, 1024);
	strcpy(name, buffer);

	while (1) {
		// read the client's query
		memset(buffer, 0, sizeof(buffer));
		valread = read(sock, buffer, 1024);
		if (valread < 0) {
		    perror("read");
		    exit(EXIT_FAILURE);
		}

		// tokenize the query string by space and retrieve the command
		char command[10];
		char* token = strtok(buffer, " ");
		strcpy(command, token);
		printf("[CMD]  %s entered command \"%s\".\n", name, command);

		// run the proper function and determine the proper response
		char response[128];
		if (strcmp(command, "quit") == 0) {
			printf("[INFO] Client %s quited.\n", name);
			break;
		} 
		else if (strcmp(command, "join") == 0) {
			token = strtok(NULL, " ");
			int group = atoi(token) - 1;
			int status = join(group, sock, name);
			if (status == 0)
				strcpy(response, "[INFO] You have been added to the group.\n");
			else
				strcpy(response, "[ERR]  Failed to add you to this group.\n");
		} 
		else if (strcmp(command, "send") == 0) {
			token = strtok(NULL, " ");
			int group = atoi(token) - 1;
			token = strtok(NULL, "\0");
			char message[128];
			strcpy(message, token);

			int status = sendMessage(group, sock, name, message);
			if (status == 0)
				strcpy(response, "[INFO] Message sent successfuly.\n");
			else
				strcpy(response, "[ERR]  Failed to send your message.\n");
		} 
		else if (strcmp(command, "leave") == 0) {
			token = strtok(NULL, " ");
			int group = atoi(token) - 1;
			leave(group, sock, name);
			strcpy(response, "[INFO] You have left the group.\n");
		}
		else
			strcpy(response, "[ERR]  Invalid command.\n");

		// send the response to client
		send(sock, response, strlen(response), 0);
	}
}

int main(int argc, char const *argv[]) {
	// check the required arguments
	if (argc < 2) {
		printf("ERR: Please enter [server-port-number] as argument.\n");
		return -1;
	}
	const int PORT = atoi(argv[1]);

	// initiate the groups
	for (int i = 0; i < MAXGROUPS; i++) {
		groups[i].groupId = i + 1;
		for (int j = 0; j < MAXUSERS; j++)
			groups[i].sockets[j] = -1, groups[i].empty[j] = 1;
	}

	// creates socket file descriptor
	int server_fd;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
	const int addrlen = sizeof(address);

	// binding
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// listening on server socket with backlog size 3.
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Listening on %s:%d...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

	while (1) {
		// accepting client
		// accept returns client socket and fills given address and addrlen with client address information.
		int client_socket, valread;
		if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("[INFO] Accepted client %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

		// create a thread for each client
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, clientHandler, (void *)&client_socket);
	}
	return 0;
}
