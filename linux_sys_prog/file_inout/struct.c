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
}
