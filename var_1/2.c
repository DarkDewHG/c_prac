#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int flag,pid;
int fd[2];

void alrmHandler(int signo)
{
	signal(SIGALRM,alrmHandler);
	int i;
	if (flag)
	{
		i=wait4(-1,NULL,WNOHANG,NULL);
		if (i<=0)
			kill(pid,SIGKILL);
		_exit(0);
	}
	int ef = EOF;
	write(fd[1],&ef,sizeof(int));
	alarm(1);
	flag=1;
}

void childHandler(int signo)
{
	signal(SIGCHLD,childHandler);
	wait(NULL);
	printf("prog ended");
}



int main(int argc, char** argv)
{
	flag=0;
	char* word[] = {"alpha\n","beta\n","gamma\n","epsilon\n","tetta\n"};
	signal(SIGCHLD,childHandler);
	signal(SIGALRM,alrmHandler);
	int i=0;
	pipe(fd);
	pid = fork();
	if (pid==0)
	{
		dup2(fd[0],0);
		close(fd[1]);
		execvp(argv[1],argv+1);
		perror(argv[1]);
		fflush(stderr);
		_exit(1);
	}
	close(fd[0]);
	alarm(5);
	char b = 54;
	for(;;)
	{
		write(fd[1],word[i],strlen(word[i]));
		i++;
		if (i==5)
			i=0;
	}

}
