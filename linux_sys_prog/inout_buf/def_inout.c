#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2

#define ERRMSG "Devide Not\n"

// 입력 받은 데이터를 atoi로 숫자로 변환하여 file로 아웃.
void file_in_print(void)
{
	int a,b;
	char buf[80];
	
	read(STDIN, buf, 80);
	a = atoi(buf);
	
	read(STDIN, buf, 80);
	b = atoi(buf);
	
	sprintf(buf, "%d / %d = %d\n", a,b,(int)(a/b));
	write(STDOUT, buf, strlen(buf));
	return ;
}

// 입출력 재지향
int inout_dup(void)
{
	int fd;
	// printf data -> test.log output
	fd=open("test.log", O_CREAT|O_WRONLY, S_IRUSR);
	fd = dup2(fd, STDOUT);
	printf("test!!");
	printf("fd %d\n",fd);
	//remove("test.log");
	return 0;
}

void inout_buf_remove(void)
{
	int i=1;
	while(1)
	{
		printf("%d",i);
		//fflush(stdout); //buf의 내용을 바로 출력하게 됨.
		usleep(100);
	}
}

int main(int argc, char* argv[])
{
	//inout_dup();
	inout_buf_remove();
	return 0;
}
