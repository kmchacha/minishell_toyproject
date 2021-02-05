#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void write_routine(char *buf);

int main(int argc, char *argv[])
{
	pid_t pid;
	char buf[BUF_SIZE];
	
	// TODO: fork
	pid = fork();
	if(pid == 0) // child
	{
		write_routine(buf);
	}
	else // parent
	{   
        int status;
        while (!waitpid(-1, &status, WNOHANG))
		{
			sleep(3);
			puts("sleep 3sec.");
		}
	}

	return 0;
}

void write_routine(char *buf)
{
    char contents[BUF_SIZE];
	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		// TODO: Write message to the echo server
		fgets(buf, BUF_SIZE-2, stdin);

		if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){
			printf("END");
			break;
		}
        sprintf(contents,"/bin/%s",buf);
        printf("%s",contents);
        execl(contents, contents, (char *) 0x0) ;
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}