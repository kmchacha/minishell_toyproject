#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define COMMAND_SIZE 30
#define MESSAGE_SIZE 30000

void error_handling(char *message);
void read_routine(int sock, char *command);
void write_routine(int sock, char *command);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char command[COMMAND_SIZE];
	char serv_message[MESSAGE_SIZE];
	struct sockaddr_in serv_adr;
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);  
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	pid = fork();
	if(pid == 0) // child
	{
		write_routine(sock, command);
	}
	else // parent
	{
		read_routine(sock, serv_message);
	}

	close(sock);
	return 0;
}

void read_routine(int sock, char *serv_message)
{
	int str_len;
	while (1)
	{
		// Read message from the server 
		str_len = read(sock, serv_message, MESSAGE_SIZE);
		if(str_len <= 0) break;

		printf("==================== Message from server ====================\n%s", serv_message);
		
	}
}
void write_routine(int sock, char *command)
{
	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		// Write command to the shell server
		fgets(command, COMMAND_SIZE-1, stdin);

		command[strlen(command)-1]=0;		// subtract '\n'
		if(!strcmp(command, "q") || !strcmp(command, "Q")){
			shutdown(sock, SHUT_WR);
			printf("END\n");
			break;
		}
		write(sock, command, COMMAND_SIZE);
		memcpy(command, 0, COMMAND_SIZE);
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}