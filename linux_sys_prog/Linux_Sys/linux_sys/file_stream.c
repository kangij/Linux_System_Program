
#include "Linux_Sys_Header.h"

#include <stdio.h>

int file_stream_ex(void)
{
    FILE *in, *out;
    size_t nr=0;
    struct test
    {
        char name[100];
        unsigned long booty;
        unsigned int beard_len;
    } p, black={"Edwared Tech",950,40};

    memset(&in,0,sizeof(in));
    memset(&out,0,sizeof(out));

    out = fopen("data","w");
    if(!out)
    {
        perror("fopen");
        return 1;
    }

    if(!fwrite(&black,sizeof(struct test),1,out))
    {
        perror("fwrite");
        return 1;
    }

    if(fclose(out))
    {
        perror("fclose");
        return 1;
    }

    in = fopen("data","r");

    if(!fread(&p,sizeof(struct test),1,in))
    {
        perror("fread");
        return 1;
    }

    if(fclose(in))
    {
        perror("fclose");
        return 1;
    }

    printf("name = %s / booty:%lu / len=%u",p.name,p.booty,p.beard_len);
    return 0;
}
