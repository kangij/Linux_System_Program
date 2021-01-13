#include "Linux_Sys_Header.h"


/*
    Scatter란? 좀더 진보된 입출력 시스템 호출 이며, 벡터형으로 변경하여 입출력을 한다.
    선형적 데이터 입출력에서 벡터형 입출력으로 변경되어 더 빠르게 변환이 가능해진다

    Scatter : writev()
    Gather  : readv()

    Data struct
    struct iovec

*/

void Scatter_IO(void)
{
    struct iovec lov[2];
    struct iovec rov[2];

    ssize_t nr;
    int fd, i;
//////////////////
    char *buf[]={
        "tpjapjepfjpapfmpafmpemfm[awmfmw[fem[amfem[ma[fwm[\n",
        "afnvnnvnnv1203081028308120380183081203801\n"
    };

    // buccan.txt 파일을 Write Only로 열고, 파일이 없으면 생성하며, 파일이 있고 쓰기 옵션으로 열었으면 초기화 함.
    fd = open("buccan.txt",O_WRONLY | O_CREAT | O_TRUNC);

    // Scatter Struct에 Write
    for(i=0;i<2;i++)
    {
        lov[i].iov_base = buf[i];
        lov[i].iov_len = strlen(buf[i]);
    }

    // 한번에 저장되어 있는 Struct 내용을 기재.
    nr = writev(fd, lov,2);
    printf( "wrote %d bytes\n",nr);

    fd = open("buccan.txt",O_RDONLY);
    char buf2[100];
    char buf3[100];
    int si=0;
    rov[0].iov_base=buf2;
    rov[0].iov_len=sizeof(buf2);
    rov[1].iov_base=buf3;
    rov[1].iov_base=sizeof(buf3);
    si= sizeof(rov) / sizeof(struct iovec);
    int ret;
    ret = readv(fd,rov,2);
    printf("\r\n %s", rov[0]);
    close(fd);
}


///////////////////////// Epoll
/*
    리눅스 전용 Multiplexed IO이다.


*/
