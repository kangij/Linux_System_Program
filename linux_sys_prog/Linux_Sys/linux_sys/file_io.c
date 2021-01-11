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
       10. O_SYNC : 동기 입출력으로 오픈 ( 메모리는 전자식 , 하드디스크는 기계식으로 동작하며 Memory가 하드디스크에 쓰이는 것보다 빠르므로, 하드디스크에 다 쓰기 까지 대기 시간을 가짐. )
       11. O_TRUNC : 파일이 존재하며, 일반적인 파일 (text/image등)에 한하여 길이를 0으로 초기화.
       12. O_RDONLY : Read Only
       13. O_WRONLY : Write Only
       14. O_RDWR   : Read / Write
       15. O_DSYNC  : 쓰기 동작 후 , Normal Data만 동기화 되도록 함. // SYNC 관련 옵션을 Open시 설정 할 경우, 모든 경우의 환경에서 SYNC를 맞춘 후 동작하므로 동작 속도가 느려질 수 있음.
       16. O_RSYNC  : 읽기와 쓰기 모두에서 동기화 되도록 한다. ( O_DSYNC or O_SYNC와 같이 사용해야함. )

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

/*//////////////////////////////////////////////////
    write function
    ssize_t write(int fd, const void *buf, size_t count);

    Parameter
        fd : File Linker
        buf : Write Data
        cout : size Data

    return parameter
        -1 = Fail
         0 = OK
*///////////////////////////////////////////////////

int m_file_write(int fd,char *file,char *f_str)
{
    ssize_t result=0;
    int len=0,ret=0;

    // 파일이 없다면 쓰기전용으로 열고, 모든 유저들에게 읽고 쓰기 권한을 준다.
    if( (fd=open(file,O_CREAT|O_WRONLY,S_IRWXU|S_IRWXG|S_IRWXO))==-1)
    {
        printf("\r\n File Open Err");
        exit(1);
    }
    else
    {
        // 연동 된 FD에서 입력된 파일
        result=write(fd,f_str,strlen(f_str));
        if(result==-1)
        {
            perror("Write ERR\r\n");
            exit(1);
        }
        else
        {
            len+=ret;
            printf("\r\n Write OK!! [%x]",len);
        }
    }

    //메모리에서 하드디스크로 저장 할때, 동작 속도가 서로 다르므로 Sync를 동기화 하여 보내는 함수.
    //하드디스크 캐시를 사용 하는 경우는 감지 불가.
    ret=fsync(fd);
    if(ret==-1)
    {
        // Error Code
        // EBADF : Wrtie 형으로 파일이 Open되지 않은 경우
        // EINVAL : file Descriptor가 동기화를 지원하지 않음
        // EIO : 동기화 동안 Low Level 입출력 오류 발생 ( 하드디스크 오류 등 )
        if(errno==EINVAL)
        {
            //하드디스크상 캐시를 사용 하는 경우, 캐시에서 하드디스크로 Write
            if(fdatasync(fd)==-1)
            {
                perror("fdatasync");
                exit(1);
            }
        }
        else
            perror("fSync Err");
        exit(1);
    }
    else
    {
        printf("\r\n fsync ok");
    }
    return fd;
}


/*//////////////////////////////////////////////////
    read function
    ssize_t read(int fd, void *buf, size_t len);

    Parameter
        fd : File Linker
        buf : read data buffer
        len : read length ( 512 or 1024 or etc )

    return parameter (EOF = end of File )
                 -1 = Fail
                  0 = EOF or Read Data가 없는 경우.
    len==return val = len 크기 만큼 읽어 들여 buf에 count 저장
    0<return val<len= 읽는 도중 중단되거나 len 크기를 모두 읽기전에 EOF에 도달.
*///////////////////////////////////////////////////

/*//////////////////////////////////////////////////
    lseek function
    파일에서 시작할 위치를 변경 해주는 기능.
    off_t lseek(int fd, off_t pos, int origin);

    Parameter
        fd : File Linker
        pos : File Position
        origin : SEEK Mode
                 1) SEEK_CUR : current + pos로 현재 위치에서 pos를 더한 위치를 설정한다. pos가 0 일경우 현재 위치를 알린다.
                 2) SEEK_END : file length + pos로 파일의 길이에 pos가 더한 위치를 설정하며, pos가 0 인경우 파일의 끝 위치로 이동
                 3) SEEK_SET : fd의 파일 위치를 pos로 설정한다. pos가 0이면 파일의 처음 시작 위치로 이동한다.
                 4) SEEK_HOLE:
    return parameter
        -1 = Fail
         0 = OK
*///////////////////////////////////////////////////
int m_file_read(int fd,char *file)
{
    char buf[1024];
    ssize_t result=0;
    off_t ret=0;
    int rn=0;
    int str_len=0;

    // 파일이 없다면 생성 후 Read Only로 Open하며, 모든 사용자들이 읽고 쓸수 있도록 만듬.
    if( (fd=open(file,O_CREAT|O_RDONLY,S_IRWXU|S_IRWXG|S_IRWXO))==-1)
    {
        perror("\r\n File Open Err");
        exit(1);
    }
    else
    {
        // Read File Example
        while( (result=read(fd,buf,1024))!=0) // 1024 or 512Kbyte로 Read 할것.
        {
            if(result==-1)
            {
                perror("Read Err");
                exit(1);
            }
            str_len+=result; // Return된 Str_len 개수.
            printf("%s",buf);
            memset(buf,0,sizeof(buf));
        }
        str_len+=result; // 최종 Return된 값을 다시 더해줌
        printf("\r\n %s / f_strlen[%d]",buf,str_len);

        // lseek Example
        ret=lseek(fd,(off_t)6,SEEK_SET); // 파일의 위치를 6으로 이동.
        if(ret==(off_t)-1)
        {
            perror("lseek(seek_set) error");
            exit(1);
        }
        memset(buf,0,sizeof(buf));
        rn=read(fd,buf,sizeof(buf));
        printf("\r\n SEEK_SET=%d , %ld , %s",rn,ret,buf);

        ret=lseek(fd,0,SEEK_CUR);
        if(ret==(off_t)-1)
            perror("SEEK_END Err");
        memset(buf,0,sizeof(buf));
        rn=read(fd,buf,sizeof(buf));
        printf("\r\n SEEK_CUR=%d , %ld , %s",rn,ret,buf);

        ret=lseek(fd,0,SEEK_END);
        if(ret==(off_t)-1)
            perror("SEEK_END Err");
        memset(buf,0,sizeof(buf));
        rn=read(fd,buf,sizeof(buf));
        printf("\r\n SEEK_END=%d , %ld , %s",rn,ret,buf);


    }
    return fd;
}

void m_file_close(int fd,char *f_name)
{
    close(fd); // File Close
    remove(f_name); // File Remove
    printf("\r\n File Close");
}



