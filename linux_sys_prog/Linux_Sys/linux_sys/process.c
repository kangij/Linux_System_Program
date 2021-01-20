
#include "Linux_Sys_Header.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// Demon Header
#include <linux/fs.h>

// Processor Test 방법
/*
    현 프로그램을 실행 후 ps -e로 프로세서 PID를 확인.
    Child Process를 kill pid로 죽여볼 것
    그러면 현 프로그램에서 감지하여 차일드 프로세서가 죽은 것을 확인 할 수 있음.
*/
void processor_satus(int pid,int stat)
{
    if(WIFEXITED(stat))
    {
        printf("\r\n Normal termination stat [%x]",WEXITSTATUS(stat));
    }

    if(WIFSIGNALED(stat))
    {
        printf("\r\n Kill signal[%x][%s]",WTERMSIG(stat),WCOREDUMP(stat)? "(dumped core)":"");
    }

    if(WIFSTOPPED(stat))
    {
        printf("\r\n Stop Signal[%x]",WSTOPSIG(stat));
    }

    if(WIFCONTINUED(stat))
    {
        printf("\r\n Continue!");
    }
}

void fork_test(void)
{
    int pid;
    int i=0; // Parent , Child Each Other Value
    int stat;
    // Child Process Create ( return : Child Precess PID Return )
    pid=fork();

    // PID

    if ( pid == -1)
    {
        perror("pork err");
        return  ;
    }
    else if(pid==0)
    {
        printf("Child Process pid [%x] ", getpid());
        while(1)
        {
            printf("cccccc(%d\n",i);
            i++;
            sleep(1);
            stat=wait(&stat); // Process 감지 기능. ( Signal이 올때 까지 대기 )
            processor_satus(pid,stat); // 외부 프로시저에서 Signal을 날릴 경우 동작.
        }
    }
    else
    {

        printf("Parent[%x]",getpid());
        printf("Child pid[%x]",pid);

        while(1)
        {
            printf("\r\n pppp(%d)",i);
            i+=4;
            sleep(1);
                        stat=wait(&stat);
            processor_satus(pid,stat);
        }
    }
    return ;
}


////Demon Test
/*
    Demon Process란?
    위 fork시 Parent에 Child가 종속되어 있는 구조로 되어 있으나, Demon Process는
    Parent의 Process에서 독립하여 Back Ground에서 구동하는 프로세서를 의미.
    Demon Process 생성 조건
    1. 부모의 Process에서 fork하고 부모 Process에서 exit(1) 함수를 사용하여 독립 시킴
    2. setisid()를 호출하여, 새로운 프로세서 그룹과 세션을 부여.
    3. chdir()을 사용하여 루트 디렉토리로 이동.
    4. 모든 File Descriptors를 Close ( open / read / write )
    5. file descriptor 0,1,2(표준입력, 출력, 오류)를 /dev/null에 연결할 것.
    위 일련의 방법을 int demon(int nochdir, int noclose)함수를 사용하여 한번에 해결 가능.
*/

int deamon_exampe(void)
{
    int fd;
    pid_t pid;
    int i;
    // Error Check는 제외 처리함.
    pid = fork();
    exit(EXIT_SUCCESS);
    setsid();
    chdir("/");

    //for(i=0;i<NR_OPEN;i++)
    for(i=0;i<3;i++)
        close(i);

    open("/dev/null",O_RDWR);
    dup(0);
    dup(0);
    return ;
}

// Test Terminal command : ps -efjc | grep daemon
void daemond_ex_test(void)
{
    int pid;
    int i=10000;

    pid=fork();
    printf("\r\n PID[%x] / [%x]",pid,getpid());

    if(pid==-1)
    {
        perror("fork err");
        exit(0);
    }
    else if(pid==0)
    {
        printf("\r\n child: pid%d\n", getpid());
        close(0);
        close(1);
        close(2);
        setsid();

        while(1)
        {
            printf("cccc(%d)\r\n",i);
            i++;
            sleep(1);
        }
    }
    else
    {
        printf("\r\n Parent pid[%x]",getpid());
        printf("\r\n child pid [%d]",pid);
        sleep(1);
        printf("\r\n Parent bye");
        exit(1);
    }
}
