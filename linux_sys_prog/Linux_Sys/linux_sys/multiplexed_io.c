
#include "Linux_Sys_Header.h"

/* Multiplexed I/O ///////////////
timerval struct로 시간을 설정.
정의 : 여러개의 입출력 장치를 동시에 사용 하기 위한 기능.
동작 방법 : 연결되어 있는 I/O 장비를 체크하여 Busy가 아닌 상태가 되면 Block 상태를 해제하여 Read / Write 기능 동작
동작 순서 : 1. 모든 장치가 Busy 상태이면 Sleep모드로 진입.
           2. Sleep중 연결된 장치들이 Busy상태가 풀림을 감지.
           3. Wakeup 상태로 전환
           4. 입출력이 가능한 Device를 Control
           5. Sleep모드 진입

종류 : Select / Poll / epoll

select : 모든 Unix 시스템에서 사용 가능한 Multiplexed I/O
poll : Select에 비해 간편한 명령어로 구분되어 있으나, Unix 시스템에 따라, 명령어가 없을 수 있음
epoll : Linux 전용.

*//////////////////////////

/* Select Function ///////////////
 int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

 prameter :
    nfds : 총 입력에 사용되는 fd_set + 1의 개수를 입력 ( 사용자가 계산해야 함. )
    readfds : Read input
    writefds : Write Ouput
    exceptfds : Expection handler
    timeout : timeval

 FD_SET(int fd, fd_set *fdset);   : fd_set 구조체에 설정.
 FD_CLR(int fd, fd_set *fdset);   : fd_set clear
 FD_ISSET(int fd, fd_set *fdset); : fd_set에 fd가 지정되어 있는지 확인.
 FD_ZERO(fd_set *fdset);          : fd_set을 초기화.
*//////////////////////////

#define TIMEOUT 5 // sec
#define BUF_LEN 1024 // Byte단위의 Read Buffer

int m_select_ex(int fd)
{
    struct timeval tv;
    fd_set readfds;
    int ret=0;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO,&readfds);

    //5초간 대기
    tv.tv_sec=TIMEOUT;
    tv.tv_usec=0;
    ret=select(STDIN_FILENO+1,&readfds,NULL,NULL,&tv);
    if(ret==-1)
    {
        perror("\r\n select err");
    }
    else if(!ret)
    {
        printf("\r\n %d seconds elapesed \n",TIMEOUT);
    }

    if(FD_ISSET(STDIN_FILENO,&readfds))
    {
        char buf[BUF_LEN+1];
        int len;

        len = read(STDIN_FILENO,buf, BUF_LEN);

        if(len==-1)
            perror("read() err\n");

        if(len)
        {
            buf[len]='\0';
            printf("read:%s\n",buf);
        }
        return 0;
    }
    fprintf(stderr, " Tis not ha\n");
    return 1;
}

int select_ex2(int fd)
{
    struct timeval tv;
    fd_set readfds;
    fd_set writefds;
    int ret;

    char buf[BUF_LEN+1];
    int len;

    tv.tv_sec=TIMEOUT;
    tv.tv_usec = 0;

_RETRY: // Goto go
    memset(buf,0,sizeof(buf));
    FD_ZERO((&readfds)); // reafds 0 Set
    FD_ZERO((&writefds));

    FD_SET(STDIN_FILENO,&readfds); // Input으로 할당
    FD_SET(STDOUT_FILENO,&writefds); // Output으로 할당

    ret=select(3,&readfds,&writefds,NULL,&tv); // 입력 갯수 + 1 , 받을 곳, 넣을 곳, Result
    if(ret==-1)
    {
        printf("\r\n select err");
        exit(1);
    }
    else if(!ret)
    {
        printf("\r\n ret! [%x]",!ret);
        return 0;
    }

    if(FD_ISSET(STDIN_FILENO,&readfds)) // File Input을 받도록 셋팅.
    {
        len = read (STDIN_FILENO,buf,BUF_LEN); // 파일을 입풋 받는다.

        if(len==-1)
        {
            printf("\r\n read(): error\n");
            return 1;
        }

        if(len)
        {
            buf[len]='\0';
            printf("read(STDION : %d : %s \n", len, buf);
        }
    }

    if(FD_ISSET(STDOUT_FILENO,&writefds))
    {
        len = write(STDOUT_FILENO,buf, sizeof(buf));

        if(len==-1)
        {
            printf("\r\n file write err");
            return 1;
        }

        if(len)
        {
            printf("write(STDOUOT) : %d : %s\n", len, buf);
        }
    }
    sleep(2);
    goto _RETRY;

    fprintf(stderr, "This shout not happ...");
    return 1;
}

/* poll Function ///////////////
 int poll(struct pollfd *ufds, unsigned char int nfds, int time-out);

 prameter :
    sturct pollfd =
    {
        int fd, // 장치식별번호
        short events;   // 이벤트 종류
        short revenets; // 수신 이벤트
 /  }

 select와 비교시
 장점
 1. 사용자가 파라미터를 계산할 필요가 없다.
 2. Large-value file descripotr에 대해선 좀더 효율이 좋다.
 3. 단일 구조체 배열로 file descriptor 집합을 정확한 크기로 생성 할 수 있다.
 4. 입력과 출력으로 분리되어 있어, 변경없이, 배열을 재사용 가능.

 단점
 1. select가 좀더 사용하기 간편하며, 일부 unix 에선 미지원
 2. timeout에 대해 select가 처리하기 편함.
*//////////////////////////

int poll_ex(int fd)
{
    struct pollfd fds[2];
    int ret;

    fds[0].fd=STDIN_FILENO;
    fds[0].events=POLLIN;

    fds[1].fd=STDOUT_FILENO;
    fds[1].events=POLLOUT;

    // 설정한 Poll struct를 2개 설정한다. 시간은 micro sec 단위.
    ret=poll(fds,2,TIMEOUT*1000);

    if(ret==-1)
    {
        printf("]r]n poll Err");
        return 1;
    }

    if(!ret)
    {
        printf("%d seconds elased\n",TIMEOUT);
        return 0;
    }

    // Receive가 된 Revnet를 점검.
    if(fds[0].revents & POLLIN)
        printf("\r\n stdin is..");
    if(fds[1].revents & POLLOUT)
        printf("\r\n stdout is writeable");

   return 0;
}


////////////////// Epoll
/*
    Linux 2.6에서 시작된 전용 Multiflex 방식이다.
    poll과 select의 제약사항을 해결하였으나 사용방식이 복잡하다.

    #include <sys/sys/epoll.h>
    int epoll create ( int size ) // 감시할 fd 개수
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

    struct epoll_event{
    __u32 evnets;
    int fd;
    __u32 u32;
    __u64 u64;
    } data;
    }

    ex)
        struct eopll_event event;
        int ret;
        event.data.fd = fd // return fd 설정
        event.events = EPOLLIN / EPOLLOUT ( or 가능 )
        ret = epoll_ctl( epfd , EPOLL_CTL_ADD / EPOLL_CTL_MOD / EPOLL_CTL_DEL, fd, &evnet )
          -> EPOLL_CTL_ADD ( 추가 ), EPOLL_CTL_MOD ( epfd에서 인스턴스로 fd로 변경 ), EPOLLOUT(epfd에서 fd목록을 삭제)
*/

#define MAX_EVENTS 16

void epoll_Task(void)
{
    int epfd, fd;
    struct epoll_event *event; // epoll event 생성
    int ret, nr_events, i;

    if( (epfd = epoll_create(10)) == -1 ) // 10개의 epoll을 받을 수 있도록 생성한다.
    {
        printf("\r\n  epoll create fail..");
        return -1;
    }

    fd=STDIN_FILENO; // file input

    struct epoll_event event1;
    event1.data.fd=fd;
    event1.events=EPOLLIN;

    // 입력되는 내용을 감지한다.
    if( (ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event1))==-1)
    {
        printf("\r\n epoll_ctl ADD err");
        return -1;
    }
#if 0
    // 화면에 출력되는 내용을 감지한다.
    fd=STDOUT_FILENO; // STD Out

    event1.data.fd=fd;
    event1.events=EPOLLOUT;


    if( (ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event1))==-1)
    {
        printf("\r\n epoll_ctl ADD1 err");
        return -1;
    }
#endif

    // event에 event를 Heap으로 생성한다.
    if((event=malloc(sizeof(struct epoll_event)*MAX_EVENTS))==-1)
    {
        printf("\r\n molloc errr");
        free(event);
        return -1;
    }

    // Event가 발생할 때 까지 (-1) 대기한다.
    if((nr_events=epoll_wait(epfd,event,MAX_EVENTS,-1))<0)
    {
        printf("\r\n epoll_wait() err");
        return -1;
    }

/*
    for(i=0;i<nr_events;i++)
    {
        printf("\r\n events[%d]=%ld on fd = %d \n",i,event[i].events,event[i].data.fd);
        ret = write(STDOUT_FILENO, buf,len);
        if(ret==-1)
        {
            printf("\r\n write() : error \n");
            return 1;
        }
    }
*/
    struct evepoll *evep; // vread /vwrite에 대한 struct가 묶여있는 스크립터

    for(i=0;i<nr_events;i++)
    {
        int what= event[i].events; // 현재 이벤트를 확인한다.
        struct event *evread = NULL, *evwrite=NULL;

        evep=(struct evepoll *)event[i].data.ptr; // event의 ptr를 체크한다.

        // 감지 되고 있는 내용을 확인한다.
        printf("\r\n events[%d]=%ld on fd=%d\n",i,what,event[i].data.fd);

        // EPOLLHUP : 연결이 종료되거나, 장애가 발생한경우 / EPOLLERR : 동작간 에러가 발생시
        if(what & (EPOLLHUP | EPOLLERR))
        {
            printf("\r\n evread_evwrite()");
        }
        else
        {
            // 입력 이벤트가 발생했다면
            if(what & EPOLLIN)
            {
                printf("\r\n evread");
            }

            // 출력 이벤트가 발생했다면.
            if(what & EPOLLOUT)
            {
                printf("evwrite");
            }
        }

        if(!(evread || evwrite))
            continue;

        if(evread!=NULL)
            printf("\r\n evread!!");

        if(evwrite!=NULL)
            printf("\r\n evwrite!!");

    }
    printf("\r\n Page Size[%d]",getpagesize());
    free(event);
}


