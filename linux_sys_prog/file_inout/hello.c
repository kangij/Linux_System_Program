#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int file_open(int fd, char* file_name)
{
	fd=open(file_name, O_CREAT|O_WRONLY,
				S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	return fd;
}

int file_write(int fd,char* file_name)
{
	fd=open(file_name,O_WRONLY);
	if(fd==-1)
	{
		perror("Write Error");
		exit(1);
	}
	else
	{
		int i=0;
		int data=0;
		int wsize=0;
		for(i=0;i<100;i++)
		{
			data=i*2;
			wsize=write(fd, (void *)&data, sizeof(int));
		}	
	}
}

void file_read(int fd,char* file_name)
{
	int readn=0;
	int buf;
	fd=open(file_name,O_RDONLY);

	while( (readn=read(fd,(void *)&buf,sizeof(buf)))>0)
	{
		printf("Read Data : %d\n", buf);
	}
}

void file_release(int fd,char *file_name)
{
	close(fd);
	remove(file_name);
}


void main(int argc, char *argv[])
{
	int fd;
	int i;
	for(i=0;i<sizeof(argv);i++)
	{	
		printf("\r\n argv[%x]=%s",i,argv[i]);
	}
	file_open(fd,argv[1]);
	file_write(fd,argv[1]);
	file_read(fd,argv[1]);
	file_release(fd,argv[1]);
}
