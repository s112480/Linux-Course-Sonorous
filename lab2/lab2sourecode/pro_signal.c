#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
 
void child_handler(int num);//子进程捕获信号后的处理函数
void parent_handler(int num);//父进程捕获信号后的处理函数
void recover_child(int num);//父进程回收子进程
void sys_err(char *str);//系统出错处理函数
 
unsigned int g_count_parent = 0;//父进程累加计数
unsigned int g_count_child = -1;//子进程累加计数
 
int main(int argc, char const *argv[])
{
	pid_t pid;
	int ret;
 
	signal(SIGCHLD,recover_child);//子进程结束或者暂停或者恢复会发送SIGCHLD信号
	signal(SIGUSR1,child_handler);//父进程给子进程发送的信号
	signal(SIGUSR2,parent_handler);//子进程给父进程发送的信号
 
	pid = fork();
	if(pid == 0)//子进程
	{
        pause();//先让父进程运行，等待父进程发来第一次信号
		printf("I am child\n");
		while(1)
		{
			ret = kill(getpid(),SIGUSR2);
			if(ret < 0)
				sys_err("kill");
			sleep(1);
		}
 
		return 0;
	}
	else if(pid > 0)//父进程
	{
		printf("I am parent\n");
		while(1)
		{
			ret = kill(pid,SIGUSR1);
			if(ret < 0)
				sys_err("kill");
			sleep(1);
		}
 
		wait(NULL);//回收进程
		return 0;
	}
	else if(pid < 0)
		sys_err("fork");
 
	return 0;
}
 
//父进程给子进程发送的信号
void child_handler(int num)
{
	printf("signal = %d g_count_child  = %d\n",num,g_count_child+=2);
}
 
//子进程给父进程发送的信号
void parent_handler(int num)
{
	printf("signal = %d g_count_parent = %d\n",num,g_count_parent+=2);
}
 
//回收子进程
void recover_child(int num)
{
	int ret = wait(NULL);
	if(ret < 0)
		sys_err("waitpid");
}
 
//系统出错处理函数
void sys_err(char *str)
{
	perror(str);
	exit(1);
}