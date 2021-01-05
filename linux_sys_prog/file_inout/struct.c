<<<<<<< HEAD
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

struct _value
{
	int val;
	char name[20];
};

typedef struct _value val=
{
	.val=20;
	.name="KANG";
};

int main(int argc, char *argv[])
{
	int i=0;
	for(i=0;i<sizeof(10);i++)
	{
		printf("\r\n argv[%x]=[%s]",i,argv[i]);
	}
	return 0;
=======

// C Lib
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Linux Sys Driver
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


enum Sys_Stat
{
	FAIL=0,
	OK
};

struct UserInfo
{
	char name[25];
	int age;
	int sex;
	char hobby[10];
};

int file_open(char *file_name)
{
	int fd;
	fd=open(file_name,O_CREAT|O_RDWR,777);
	if(fd==-1)
	{
		perror("ERR");
		exit(1);
	}
	
	return fd;
}

int file_write(int fd,char *file_name, struct UserInfo Info)
{
	if( (fd=open(file_name,O_WRONLY,777))==-1)
	{
		perror("ERRR");
		exit(1);
	}
	if( -1 == write(fd,(void*)&Info,sizeof(Info)))
	{
		perror("Write Err");
		exit(1);
	}
	return fd;
}

int file_read(int fd,char *file_name)
{
	int buf;
	struct UserInfo read_Info;

	if((fd=open(file_name,O_RDONLY,777))==-1)
	{	
		perror("Read err");
		exit(1);
	}

	if( -1 == read(fd,(void*)&read_Info, sizeof(read_Info)) )
	{
		perror("ERRR READ");
		exit(1);
	}
	else
	{
		printf("\r\n Name : %s ", read_Info.name);
		printf("\r\n Age  : %d ", read_Info.age);
		printf("\r\n sex  : %d ", read_Info.sex);
		printf("\r\n hobby: %s ", read_Info.hobby);
	}	
	return OK;
}

void release(int fd,char *file_name)
{
	close(fd);
	remove(file_name);
}

int main(int argc, char* argv[])
{
	struct UserInfo Info={"KANG",1,2,"NO"};
	int fd=0;
	fd=file_open(argv[1]);
	file_write(fd,argv[1],Info);
	file_read(fd,argv[1]);
	//close(fd);
	release(fd,argv[1]);
>>>>>>> refs/remotes/origin/main
}
