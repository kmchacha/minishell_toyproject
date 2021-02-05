#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	pid_t pid;
	char command[BUF_SIZE], buf[BUF_SIZE];
	char *option[30], opt_cnt=0;
    while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(command, BUF_SIZE-1, stdin);
        command[strlen(command)-1] = 0; // Subtract "\n"

		if(!strcmp(command, "q") || !strcmp(command, "Q")){
			printf("END");
			break;
		}

        // // command가 입력되면 옵션 띄어쓰기 단위로 나누기
        // char *ptr = strtok(command, " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
        // while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
        // {
        //     option[opt_cnt] = ptr;
        //     printf("%s %s", ptr, option[opt_cnt]);          // 자른 문자열 출력
        //     opt_cnt++;
        //     ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
        // }

        sprintf(buf,"/bin/%s",command);
        printf("%s\n", buf);
        execl(buf, command,(char *)0x0);
        //execl("/bin/ls", "ls", "-al",(char *)0x0);
	}

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}