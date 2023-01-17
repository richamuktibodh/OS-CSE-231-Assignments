#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include<sched.h>
  
int main()
{
    int pid, pid1, pid2;
    int ret1, ret2, ret3;
    int prio1, prio2,prio3;
    struct sched_param param1, param2, param3;

    pid = fork();
  
    if (pid == 0) {
        prio1 = 0;
        param1.sched_priority = prio1;
        ret1 = sched_setscheduler(pid,SCHED_OTHER, &param1);
        printf("child[1] --> pid = %d and ppid = %d\n",getpid(), getppid());
        execl("./actual.sh", NULL); 
    }
  
    else {
        wait(NULL);
        pid1 = fork();
        if (pid1 == 0) {
            prio2 = 20;
            param2.sched_priority = prio2;
            ret2 = sched_setscheduler(pid1,SCHED_FIFO, &param2);
            printf("child[2] --> pid = %d and ppid = %d\n",getpid(), getppid());
            execl("./actual.sh", NULL); 

        }
        else {
            wait(NULL);
            pid2 = fork();
            if (pid2 == 0) {
                prio3 = 30;
                param3.sched_priority = prio3;
                ret3 = sched_setscheduler(pid2,SCHED_RR, &param3);  // returning 0
                printf("child[3] --> pid = %d and ppid = %d\n",getpid(), getppid());
                execl("./actual.sh", NULL); 
            }

            else {
                wait(NULL);
                printf("parent --> pid = %d\n", getpid());
            }
        }
    }
  
    return 0;
}
