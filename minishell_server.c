#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h> 

#define COMMAND_SIZE 30
#define BUF_SIZE 30
#define OPTION_SIZE 30

void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char command[COMMAND_SIZE], buf[BUF_SIZE];
    char **new_argv; 
    int idx; 

    new_argv = (char **)malloc(sizeof(char *) * OPTION_SIZE);
	
    if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	while (1)
	{
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected...");
        
		pid = fork();
		if (pid == -1)
		{
			close(clnt_sock);
			continue;
		}
		if (pid == 0) // child -> echo
		{
			close(serv_sock);

			while ((str_len = read(clnt_sock, command, COMMAND_SIZE)) != 0)
			{	
				//dup2(clnt_sock, STDOUT_FILENO);
				printf("Received from client : %s\n",command);
                command[str_len] = 0;

                // command가 입력되면 옵션 띄어쓰기 단위로 나누기
                char *ptr = strtok(command, " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
                idx = 0;
                while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
                {
                    new_argv[idx] = ptr;
                    ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환

                    idx++;
                }

				dup2(clnt_sock, STDOUT_FILENO);
				sprintf(buf, "/bin/%s", new_argv[0]);
				if(execv(buf, new_argv) == -1){
					fprintf(stdout, "프로그램 실행 error: %s\n", strerror(errno));
					return -1;
				}
			}

		}
		else // parent
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}