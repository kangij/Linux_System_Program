
#include "Linux_Sys_Header.h"

#include <stdio.h>

int file_stream_ex(void)
{
    FILE *in, *out;

    struct test
    {
        char name[100];
        unsigned long booty;
        unsigned int beard_len;
    } p, black={"Edwared Tech",950,40};

    memset(&in,0,sizeof(in));
    memset(&out,0,sizeof(out));

    // data란 파일을 Write Only로 Open해라. 없으면 생성
    out = fopen("data","w");
    if(!out)
    {
        perror("fopen");
        return 1;
    }

    // fwrite에 black struct 1개를 output한다 ( 파일에 Write한다 )
    if(!fwrite(&black,sizeof(struct test),1,out))
    {
        perror("fwrite");
        return 1;
    }

    // out을 Close하여, DisConnection한다. 종료전 Fulsh(완료)를 하고 종료한다.
    if(fclose(out))
    {
        perror("fclose");
        return 1;
    }

    // data란 파일을 read 형태로 open한다.
    in = fopen("data","r");

    //struct test만큼의 struct를 p에 read한다.
    if(!fread(&p,sizeof(struct test),1,in))
    {
        perror("fread");
        return 1;
    }

    // file close
    if(fclose(in))
    {
        perror("fclose");
        return 1;
    }

    printf("name = %s / booty:%lu / len=%u",p.name,p.booty,p.beard_len);
    return 0;
}


/////////////////////////////////// Ex2

FILE* fopen_mod(char *fname, int mod)
{
    FILE *stm;

    switch(mod)
    {
        case 0:
            stm = fopen(fname,"r");
        break;

        case 1:
            stm = fopen(fname,"r+");
        break;

        case 2:
            stm = fopen(fname,"w");
        break;

        case 3:
            stm=fopen(fname,"w+");
        break;

        case 4:
            stm=fopen(fname,"a");
        break;

        case 5:
            stm=fopen(fname,"a+");
        break;
    }
    if(!stm)
        printf("\r\n file open error");
    return stm;
}

void file_stream_ex2_task(void)
{
    FILE *stm;
    int i,c;
    char s[LINE_MAX];
    char *data[]={"apple\n","banana\n","car\n","dragon\n","egloo\n","fox\n"};

    //Linux가 다룰수 있는 최대 입력 행의 길이 -> "limits.h"을 추가하여 사용한다.
    printf("LINE_MAX = %d\n",LINE_MAX);

    for(i=0;i<6;i++)
    {
        printf("--%d--\n",i);
        // fopen의 모든 입력 출력 방식을 테스트 한다.
        stm = fopen_mod("ch_data",i);
        if(stm)
        {
            // 한 단어 씩 전달한다.
            fputc(*data[i],stm);
            // 한줄 씩 전달한다.
            fputs(data[i],stm);

            // 한단어씩 가져온다.
            c=fgetc(stm);
            // 저장할 곳 , 읽어드릴 최대 개수, 읽어드릴 곳.
            if(!fgets(s,LINE_MAX,stm))
                printf("\r\n fget error");
            // 파일 종료
            fclose(stm);
        }
    }
    printf("\r\n ---- data -----");
    // FILE에 read only로 읽어 들인다.
    stm=fopen_mod("ch_data",0);
    // fgets로 모든 저장되어 있는 스펠링 불러 드린다.
    while(fgets(s,LINE_MAX,stm))
        printf("\r\n %s",s);
    // 모든 File Descrpition을 flush해주고 close한다.
    fcloseall();
}

//////////////////////////////////////////////// File Buffering Control
/*
 버퍼링이란? 파일을 실행 시 모든 데이터를 받아 사용해야하는데 이러면 속도 측면에서 느려지므로, 실시간으로 보기 위해
            일부 데이터를 전송 받아 출력하는 방식을 말한다.
 _IONBF(unbuffered) : 데이터를 직접 커널에 전송한다. ( 거의 사용하지 않음 )
 _IOLBF(Line-buffer) :  행단위 기반으로 버펄이을 수행한다. 화면에 데이터를 출력하는 스트림에 유용
 _IOFBF(Block-buffered) : 블럭 기반으로 버퍼링을 수행하며, 파일을 다룰 때 적합.
*/


