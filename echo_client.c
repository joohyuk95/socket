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
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in server_address;
	
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
		puts("connected............");
	
	while(1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		break;
		
		write(client_socket, message, strlen(message));
		str_len = read(client_socket, message, BUF_SIZE - 1);
		message[str_len] = 0;
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
