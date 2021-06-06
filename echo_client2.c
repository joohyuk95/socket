#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int client_socket;
	int str_len, receive_len, receive_complete;
	
	struct sockaddr_in server_address;
	char message[BUF_SIZE];
	
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
		error_handling("socket() error");
	
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(argv[1]);
	server_address.sin_port = htons(atoi(argv[2]));
	
	if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
		error_handling("connect() error");
	else
		printf("Connected..............");
	
	while(1) {	
		printf("Input message(Q to quit):");
		fgets(message, BUF_SIZE, stdin);	
		
		if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
			break;
		
		str_len = write(client_socket, message, strlen(message));
		
		receive_len = 0;
		while (str_len < receive_len) {
			receive_complete = read(client_socket, &message[receive_len], BUF_SIZE-1);
			if (receive_complete == -1)
				error_handling("read() error");
			receive_len += receive_complete;
		}
		message[receive_len] = 0;
		printf("Message from server: %s", message);	
	}
	close(client_socket);
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
