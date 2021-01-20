
#include "Linux_Sys_Header.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/errno.h"
#include "sys/mman.h"
#include <unistd.h>

void mmap_test(char *file_name, int argc)
{
    struct stat sb;
    off_t len;
    char *p;
    int fd;

    printf("\r\n Memory Page Size[%d]",getpagesize());

    fd = open(file_name,O_RDONLY);
    if(fd==-1)
    {
        perror("open");
        return 1;
    }

    if(fstat(fd,&sb)==-1)
    {
        perror("fstat");
        return 1;
    }

    if(!S_ISREG(sb.st_mode))
    {
        fprintf(stderr, "%s is not a file\n",file_name);
        return 1;
    }

    p=mmap(0,sb.st_size,PROT_READ, MAP_SHARED, fd, 0);

    close(fd);

    for(len = 0 ; len <sb.st_size;len++)
        putchar(p[len]);

    munmap(p,sb.st_size);
    return 0;
}
