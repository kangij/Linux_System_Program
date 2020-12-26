// C Lib
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Linux Lib
#include <unistd.h>

#define OPEN_DIR "test.txt"

FILE* file_open(FILE* fp)
{
	fp=fopen(OPEN_DIR,"a+");
	if(fp==NULL)
	{	
		perror("ERRR");
		exit(0);
	}

	return fp;
}

FILE* file_seek(FILE* fp)
{
	char buf[256];
	fseek(fp,3,SEEK_SET);
	fgets(buf,256,fp);
	printf("%s",buf);	
	fseek(fp,0,SEEK_SET);
	fgets(buf,256,fp);
	printf("%s",buf);

	return fp;	
}

int main(int argc, char **argv)
{
	FILE* fp;
	fp=file_open(fp);
	printf("%ld\n\n", ftell(fp));

	file_seek(fp);
	fclose(fp);
}
