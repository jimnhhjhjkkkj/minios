#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

void ipc()
{
    char write_msg[BUFFER_SIZE] = "Greetings.";
    char read_msg[BUFFER_SIZE];
    int status;
    int fd[2];
    pid_t pid;
    
    if (pipe(fd) == -1)
    {
    	printf("Pipe failed.");
    	exit(0);
    }
    
    pid = fork();
    
    if (pid < 0)
    {
    	printf("Fork Failed.");
    	exit(0);
    }
    else if (pid == 0)		//child
    {
    	close(fd[WRITE_END]);
	read(fd[READ_END], read_msg, BUFFER_SIZE);
	printf("read %s\n", read_msg);
    	close(fd[READ_END]);
    	exit(0);
    }
    else
    {
    	close(fd[READ_END]);
    	write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
    	close(fd[WRITE_END]);
    	wait(&status);
    }
}
