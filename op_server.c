#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 4
#define OPERAND_SIZE 4

void error_handling(char *message);
int calculate(int opnum, int operand[], char operator);

int main(int argc, char *argv[])
{
	int server_socket, client_socket;
	char operation_input[BUF_SIZE];
	int result, operand_numbers;
	int receive_complete, receive_len;
	struct sockaddr_in server_address, client_address;
	socklen_t client_address_size;
	
	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		error_handling("socket() error");
	
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(atoi(argv[1]));
	
	if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
		error_handling("bind() error");
	
	if (listen(server_socket, 5) == -1)
		error_handling("listen() error");
	
	client_address_size = sizeof(client_address);
	
	for (int i = 0; i < 5; ++i) {
		client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_size);
		if (client_socket == -1)
			error_handling("accept() error");
		read(client_socket, &operand_numbers, 1);
		
		receive_len = 0;
		while (operand_numbers*OPERAND_SIZE + 1 > receive_len) {
			receive_complete = read(client_socket, &operation_input[receive_len], BUF_SIZE-1);
			receive_len += receive_complete;
		}
		result = calculate(operand_numbers, (int *)operation_input, operation_input[receive_len-1]);
		write(client_socket, (char *) &result, sizeof(result));
		close(client_socket);
	}
	close(server_socket);
	return 0;
}

int calculate(int opnum, int operand[], char operator)
{
	int result = operand[0], i;
	
	switch (operator) {
		case '+':
			for (i = 1; i < opnum; ++i)
				result += operand[i];
			break;
		case '-':
			for (i = 1; i < opnum; ++i)
				result -= operand[i];
			break;
		case '*':
			for (i = 1; i < opnum; ++i)
				result *= operand[i];
			break;
	}
	return result;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
