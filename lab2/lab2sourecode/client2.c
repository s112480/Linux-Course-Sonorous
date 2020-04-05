#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include<memory.h>
#define BUFFSIZE 100

int main()
{
    const char *fifo_name = "/tmp/my_fifo";
    int pipe_fd = -1;
    int data_fd = -1;
    int res = 0;
    int open_mode = O_RDONLY;
    char buffer[PIPE_BUF + 1];
    int bytes_read = 0;
    int bytes_write = 0;
	
    memset(buffer, '\0', sizeof(buffer));

    printf("Process %d opening FIFO O_RDONLY\n", getpid());
    pipe_fd = open(fifo_name, open_mode);
    data_fd = open("DataFormFIFO.txt", O_WRONLY|O_CREAT, 0644);
	
	pid_t pid;
	int pfd[2];
	char buff[BUFFSIZE];
	int rdnum,wtnum;
	memset(buff,0,sizeof(buff));
	if(pipe(pfd)<0){
		perror("pipe error");
		exit(1);
	}
	pid=fork();
	if(pid<0){
		perror("fork error");
		exit(1);
	}else if(pid==0){
		close(pfd[1]);
		sleep(1);
		rdnum=read(pfd[0],buff,BUFFSIZE);
		printf("read %d bytes from the pipe:%s\n",rdnum,buff);
		close(pfd[0]);
		exit(0);
	}else{
		close(pfd[0]);
		
		
		if (data_fd == -1)
		{
			fprintf(stderr, "Open file[DataFormFIFO.txt] failed\n");
			close(pipe_fd);
			return -1;
		}

		printf("Process %d result %d\n",getpid(), pipe_fd);
		if(pipe_fd != -1)
		{
			do
			{
				res = read(pipe_fd, buffer, PIPE_BUF);
				bytes_write = write(data_fd, buffer, res);
				bytes_read += res;
			}while(res > 0);

			close(pipe_fd);
			close(data_fd);
			
		}
		else
			exit(EXIT_FAILURE);

		printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);
		wtnum=write(pfd[1],"pipe-",5);
		printf("write %d bytes to the pipe.\n",wtnum);
		wtnum=write(pfd[1],buffer,bytes_read);
		printf("write %d bytes to the pipe.\n",wtnum);
		
		close(pfd[1]);
		waitpid(pid,NULL,0);
		exit(0);
	}
    

    
}