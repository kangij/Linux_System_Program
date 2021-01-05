#include "Linux_Sys_Header.h"

/* File Open ///////////////
int open(const char *name, int flags, mode_t mod);

parameter :
    flags
        1. O_ASYNC  : terminal & soket sync use
        2. O_CREAT  : No file -> Create
        3. O_DIRECT : Direct File IN/OUT Open
        4. O_DIRECTORY : DIRECOTRY Open
        5. O_EXCL : O_CREAT | O_EXCL Use -> already file -> return fail(-1)
        6. O_LARGEFILE : 64BIT architecture Use -> file size > 2GB use
        7. O_NOCTTY : Terminal Driver (/dev/tty) no Process
        8. O_NOFOLLOW : 심볼릭 링크를 오픈 할 때, 실패 반환.
        9. O_NONBLOCK , O_NDELAY : 파일을  None Block 상태로 Open
       10. O_SYNC : 동기 입출력으로 오픈 ( 메모리는 전자식 , 하드디스크는 기계식으로 동작하며 Memory가 하드디스크에 쓰이는 것보다 빠르므로, 하드디스크에 다 쓰기 까지 대기 시간을 가짐.
       11. O_TRUNC : 파일이 존재하며, 일반적인 파일 (text/image등)에 한하여 길이를 0으로 초기화.
       12. O_RDONLY : Read Only
       13. O_WRONLY : Write Only
       14. O_RDWR   : Read / Write

    mod ( authority )

                     S_IRWXU     S_IRUSR     S_IWUSR     S_IXUSR     S_IRWXG     S_IRGRP     S_IWGRP     S_IXGRP     S_IRWXO     S_IROTH     S_IWOTH     S_IXOTH
        READ            O           O                                   O           O                                   O           O
        WRITE           O                       O                       O                       O                       O                       O
        EXCUTE          O                                   O           O                                   O           O                                   O


    return parameter
    EEXIST : O_CREAT과 O_EXCL을 동시 사용 한경우, 파일이 있을 때의 return 값
    EACESS : 파일 접근이 불가 할 때
    ENOENT : 경로에 디렉토리가 없거나 심볼이 사용 불가 할 때.
    ENODEV : 경로에 장치 파일을 참고 하였으나, 일치하는 장비가 없을 때.
    EROFS  : Read Only 파일에 쓰기를 하려 할 때.
    EFAULT : 경로명에 접근 할 수 없는 주소로 접근 할 때.
    ELOOP  : 심볼이 너무 많을 때.
*//////////////////////////

void m_file_write(char *file)
{
    int fd=0;
    ssize_t result=0;

    if( (fd=open(file,O_CREAT|O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO))==-1)
    {
        printf("\r\n File Open Err");
        exit(1);
    }
    else
    {
        result=write(fd,file,strlen(file));
        if(result==-1)
        {
            perror("Write ERR\r\n");
            exit(1);
        }
        else
        {
            printf("\r\n Write OK!!");
        }
    }
}

void m_file_read(char *file)
{
    int fd=0;
    char buf[1024];
    ssize_t result=0;

    if( (fd=open(file,O_CREAT|O_RDONLY,S_IRWXU|S_IRWXG|S_IRWXO))==-1)
    {
        perror("\r\n File Open Err");
        exit(1);
    }
    else
    {
        while( (result=read(fd,buf,1024))!=0)
        {
            if(result==-1)
            {
                perror("Read Err");
                exit(1);
            }
        }
        printf("\r\n %s",buf);
    }
    close(fd);
    remove(file);
}


