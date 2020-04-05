#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
 
int pid;
 
void first_son_process() {
   int count = 0;
   int pid;
   pid = fork();
   if (pid < 0) {
       puts("fork fail");
       exit(1);
   } else if ( pid == 0) {
     printf("I am first child process, tht count is %d, my process id is %d\n", count, getpid());
   } else {
     printf("I am first parent process, tht count is %d, my process id is %d, son id %d\n", ++count, getpid(), pid);
   }
}
void second_son_process() {
   int count = 0;
   int pid = vfork();
   if (pid < 0) {
      puts("vfor fail");
   } else if (pid == 0) {
      printf("I am second child process, the count is %d, my process id is %d\n",  ++count, getpid());
      exit(1);
   } else {
      printf("I am second parent process, the count is %d, my process id is %d, son id %d\n", count, getpid(), pid);
   }
}
int main()
{
    first_son_process();
//    second_son_process();
    return 0;
}