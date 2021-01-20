#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// kill -l 로 list 확인.
static void signal_handler(int signo)
{
    switch(signo)
    {
        case SIGINT:
            printf("\r\n Signal int");
        break;

        case SIGTERM:
            printf("\r\n SIGNAL TERM");
        break;

        case SIGHUP:
            printf("\r\n SIGNAL HUP");
        break;

        case SIGUSR1:
            printf("\r\n SIGUSR1");
        break;

        default:
            printf("\r\n unkown...SIG");
        break;
    }
}

void signal_ex(void)
{
        int pid, child;

        signal(SIGUSR1,signal_handler); // SIGUSR1 동작 Function을 설정한다.

        pid = fork();
        child=pid;

        printf("\r\n child pid[%d]",child);
        printf("\r\n ddddd");

        while(1)
        {
            sleep(5);
            raise(SIGUSR1);// 인자로 설정된 시그널로 인자를 보낸다. signal handler 수행이 종료될때까지 대기

            sleep(2);
            kill(child,SIGUSR1); // child를 Kill한다.
        }
}
