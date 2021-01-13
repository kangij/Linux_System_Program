#include "Linux_Sys_Header.h"

int main(int argc,char *argv[])
{

#if 0
    int fd=0;
    char *str="HELLO 2021";

    fd=m_file_write(fd,argv[1],str);
    sync(); // 모든 장치들을 동기화 시킴 ( 매번 동작 시 시스템에 과부화를 주므로, 작업이 마무리되는 시점에 1회 사용하는것이 효과적 )
    fd=m_file_read(fd,argv[1]);
    m_select_ex(fd);
    //select_ex2(fd);
    poll_ex(fd);
    m_file_close(fd,argv[1]);
#endif // 0

    //Serial_Task();
    //file_stream_ex();
    //file_stream_ex2_task();
    //Scatter_IO();
    epoll_Task();
    return 0;
}
