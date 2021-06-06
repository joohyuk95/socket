#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int client_socket;
	struct sockaddr_in server_address;
	char message[30];
	int str_len = 0;
	int idx = 0, read_len = 0;
	
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
		error_handling("socket error");
	
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(argv[1]);
	server_address.sin_port = htons(atoi(argv[2]));
	
	if ( connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
		error_handling("connect error");
	
	while (read_len = read(client_socket, &message[idx++], 1)) {
		if (read_len == -1)
			error_handling("read error");
		
		str_len += read_len;
	}	
	
	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", str_len);
	close(client_socket);
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
