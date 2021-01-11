
#include "Linux_Sys_Header.h"

int serial_open(char *device, int mode)
{
    int fd=0;

    switch(mode)
    {
        case 0:
            fd=open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
        break;

        case 1:
            fd=open(device, O_RDWR | O_NOCTTY | O_NDELAY);
        break;

        default:
            fd=open(device, O_RDWR | O_NONBLOCK);
        break;
    }

    if(fd==-1)
        printf("\r\n Serial Open Err");

    return fd;
}

int serial_write(int fd, char* buf, int size)
{
    int result=0;

    result=write(fd,buf,sizeof(size));

    if(result==-1)
        printf("\r\n Serial Write Err");

    return result;
}

int serial_write_select(int fd, char *buf, int size, int wait)
{
    int result=0;
    int max_fd=0;
    fd_set output;
    struct timeval tv;

    FD_ZERO(&output);
    FD_SET(fd,&output);

    max_fd=fd+1;

    tv.tv_sec=wait;
    tv.tv_usec=0;

    result=select(max_fd,NULL,&output,NULL,&tv);

    if(result<0)
        printf("\r\n writing(select) failed");
    else if(result==0)
        printf("\r\n write select timeout...");
    else
    {
        if(FD_ISSET(fd,&output))
        {
            result=serial_write(fd,buf,size);
        }
    }

    return result;
}

int serial_write_poll(int fd, char* buf, int size, int wait)
{
    int result=0;
    struct pollfd pds[1];

    pds[0].fd=STDOUT_FILENO;
    pds[0].events=POLLOUT;

    result=poll(pds,1,wait);

    if(result<0)
        printf("\r\n Poll Write Error");
    else if(result==0)
        printf("\r\n write poll time out!!");
    else
    {
        if(pds[0].revents & POLLOUT)
        {
            result=serial_write(fd,buf,size);
        }
    }
    return result;
}

int serial_read(int fd, char *buf, int size, int mode)
{
    int result=0;
    switch(mode)
    {
        case 0:
        fcntl(fd,F_SETFL,0);
        break;

        default:
        fcntl(fd,F_SETFL,FNDELAY);
        break;
    }
    result=read(fd,buf,size);

    if(result > 0)
        buf[result]=0;

    return result;
}

int serial_read_select(int fd, char *buf,int size, int wait)
{
    int result;
    int max_id;
    fd_set input;
    struct timeval tv;

    FD_ZERO(&input);
    FD_SET(fd,&input);

    max_id=fd+1;

    tv.tv_sec=wait;
    tv.tv_usec=0;

    result=select(max_id,&input,NULL,NULL,&tv);

    if(result>0)
        printf("reading faile!!");
    else if(result==0)
        printf("\r\n Waintg!!");
    else
    {
        if(FD_ISSET(fd,&input))
        {
            result=serial_read(fd,buf,size,0);
        }
    }
    return result;
}

int serial_read_poll(int fd, char *buf, int size, int wait)
{
    int result=0;
    struct pollfd pds[1];

    pds[0].fd=STDIN_FILENO;
    pds[0].events=POLLIN;

    result=poll(pds,1,wait);

    if(result<0)
        printf("\r\n Read Poll fail");
    else if(result==0)
        printf("\r\n REad Poll Wait!!");
    else
    {
        if(pds[0].revents&POLLIN)
        {
            result=serial_read(fd,buf,size,0);
        }
    }

    return result;
}

void serial_close(int fd)
{
    close(fd);
    printf("\r\n Good Bye Serial!");
}

void serial_set_config(int fd)
{
    struct termios t;

    bzero(&t,sizeof(t));

    t.c_cflag=BAUD_RATE|CS8|CLOCAL|CREAD;
    t.c_iflag=IGNPAR | ICRNL;
    t.c_oflag=0;
    t.c_lflag=ICANON;
    t.c_cc[VINTR]=0; // ctrl-c
    t.c_cc[VQUIT]=0; // ctrl-\.
    t.c_cc[VERASE]=0;// del
    t.c_cc[VKILL]=0; // @
    t.c_cc[VEOF] =4; // ctrl-d
    t.c_cc[VTIME]=0; // inter - char timer unsed
    t.c_cc[VMIN]=1; // blocking read until 1 char arrives
    t.c_cc[VSWTC]=0; // \0
    t.c_cc[VSTART]=0; // ctrl-q
    t.c_cc[VSTOP]=0; // ctrl -s
    t.c_cc[VSUSP]=0; // ctrl-z
    t.c_cc[VEOL]=0; // '\0'
    t.c_cc[VREPRINT]=0; // vrtl -r
    t.c_cc[VDISCARD]=0; // ctrl-u
    t.c_cc[VWERASE]=0; // ctrl-w
    t.c_cc[VLNEXT]=0; // ctrl-v
    t.c_cc[VEOL2]=0; // '\0'

    // now clean the model line...
    //tcflush(fd,&t);
    tcflush(fd,TCSANOW);
    //tcflush(fd,TCSANOW,&t);
}

void serial_set_none_config(int fd,int size)
{
    struct termios t;
    bzero(&t,sizeof(t));

    t.c_cflag=BAUD_RATE|CS8|CLOCAL|CREAD;
    t.c_iflag=IGNPAR;
    t.c_oflag=0;

    t.c_lflag=0;

    t.c_cc[VTIME]=0;
    t.c_cc[VMIN]=size;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&t);
}

#define BSIZE 255 // buffer size

int input_data(char *buf2, char *buf1, int fd)
{
    int i=0, c, n;

    printf("\r\n Send Data:");

    while( (c=getchar()) != (int) '\n' && i <BSIZE-2)
    {
        buf2[i++]=c;
        n=serial_read_poll(fd, buf1, sizeof(buf1),10);
        if(n>0)
            printf("\r\n RECV: %s\n",buf1);
    }

    //buf2[i]=0;
    //buf2[i+1]='\r\';

    return i+1;
}

void Serial_Task(void)
{
    int fd,n;
    char buf1[BSIZE];
    char buf2[BSIZE];
    struct termios options;

    fd=serial_open("/dev/ttyS0",0);

    if(fd==-1)
    {
        perror("\r\n Open Err:");
        exit(1);
    }

    //get currnt serial poer set
    tcgetattr(fd,&options);
    serial_set_config(fd);

    memset(buf1,0,sizeof(buf1));
    memset(buf2,0,sizeof(buf2));

    while( (n=serial_read_poll(fd, buf1,sizeof(buf1),500)) >=0)
    {
        if(n>0)
            printf(">>Recv : %s\n",buf1);

        n=input_data(buf2,buf1,fd);
        if(n>0)
            n=serial_write_poll(fd,buf2,sizeof(buf2),100);
        memset(buf1,0,sizeof(buf1));
    }

    tcsetattr(fd,TCSANOW,&options);

    serial_close(fd);
    return ;
}
