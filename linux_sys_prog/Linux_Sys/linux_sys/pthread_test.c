#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// complie시 -lpthread 를 추가 해야한다.
void print(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid=getpid();
    tid=pthread_self();
    printf("\r\n %s pid =%u, tid =%u(0x%x)",s,(unsigned int)pid,(unsigned int)tid, (unsigned int)tid);
}

void *th_ex2(void *arg)
{
    print("new thread:");
    return ((void*)0);
}

void th_ex_task(void)
{
    int err;
    int id;
    int param=100;
    pthread_t tidp;

    id=pthread_create(&tidp, NULL, th_ex2, &param);

    print("main thread:");
    sleep(1);

    while(1)
    {
        pause();
    }
}
