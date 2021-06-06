#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RESULT_SIZE 4
#define OPERAND_SIZE 4

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int client_socket;
	char operation_input[BUF_SIZE];
	int result, operand_numbers;
	struct sockaddr_in server_address;
	
	if (argc != 3) {
		printf("Usage : %s <IP> <port> \n", argv[0]);
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
		printf("Connected..........\n");
	
	printf("Operand numbers : ");
	scanf("%d", &operand_numbers);
	operation_input[0] = (char)operand_numbers;
	
	for (int i = 0; i < operand_numbers; ++i) {
		printf("operand %d: ", i + 1);
		scanf("%d", (int *) &operation_input[i*OPERAND_SIZE + 1]);
	}
	fgetc(stdin);
	fputs("operator: ", stdout);
	scanf("%c", &operation_input[operand_numbers*OPERAND_SIZE +1]);
	write(client_socket, operation_input, operand_numbers*OPERAND_SIZE + 2);
	read(client_socket, &result, RESULT_SIZE);
	
	printf("Operation result : %d\n", result);
	close(client_socket);
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
