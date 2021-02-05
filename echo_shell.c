#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h> 

#define BUF_SIZE 30
#define OPTION_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
	pid_t pid;
	char command[BUF_SIZE], buf[BUF_SIZE];
	char **new_argv; 
    int idx = 0; 

    new_argv = (char **)malloc(sizeof(char *) * OPTION_SIZE);
    
    while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(command, BUF_SIZE-1, stdin);
        command[strlen(command)-1] = 0; // Subtract "\n"

		if(!strcmp(command, "q") || !strcmp(command, "Q")){
			printf("END");
			break;
		}

        // command가 입력되면 옵션 띄어쓰기 단위로 나누기
        char *ptr = strtok(command, " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
        while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
        {
            new_argv[idx] = ptr;
            ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환

            idx++;
        }

        sprintf(buf, "/bin/%s", new_argv[0]);
        if(execv(buf, new_argv) == -1){
            fprintf(stdout, "프로그램 실행 error: %s\n", strerror(errno));
            return -1;
        }
	}

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}