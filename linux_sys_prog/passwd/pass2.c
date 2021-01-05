#include <unistd.h>
#include <utmp.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h> 
#include <fcntl.h>
#include <string.h> 
#include <stdlib.h>

int get_current_pid(int login_pid);
char *get_current_procname(int proc_num);

int main(int argc, char **argv)
{
    struct utmp *utmpfp;
    struct utmp *myutmp;

    char *tty_name = NULL;
    char *host_ip; 
    
    char now_local_time[50];

    time_t the_time;
    struct tm *tm_ptr;

    setutent();

    // getutent()를 이용해서 utmp정보를 얻어온다.
    while ((utmpfp = getutent()) != NULL)
    {
        // ut_time 은 유닉스 시간으로 저장되는데, localtime 함수를 이용해서
        // 우리가 쉽게 읽을 수 있는 시간으로 변경했다.    
        the_time = utmpfp->ut_time;
        tm_ptr = localtime(&the_time);
        sprintf(now_local_time, "%d/%02d/%02d %02d:%02d",
                                tm_ptr->tm_year+1900, tm_ptr->tm_mon+1,
                                tm_ptr->tm_mday, tm_ptr->tm_hour,
                                tm_ptr->tm_min);

        host_ip = utmpfp->ut_host;
        if (strlen(host_ip) < 1)
            host_ip = "-";
        if (utmpfp->ut_type == USER_PROCESS)
        {
            printf("%-12s %-8s %-12s %s   %s\n",
                        utmpfp->ut_user, 
                        utmpfp->ut_line,
                        host_ip,
                        now_local_time,
                        get_current_procname(get_current_pid(utmpfp->ut_pid)));
        }
    }


    return 1;
}

int get_current_pid(int login_pid)
{
    int  fd;
    char buf[255];
    char stat_file[25];
    int  field_num = 7;
    int  i, j;
    int  buf_index;
    char current_pid[11];

    memset(buf, '\0', 255);
    sprintf(stat_file, "/proc/%d/stat", login_pid);

    if ((fd = open(stat_file, O_RDONLY)) == -1)
    {
        printf("error\n");
        return -1;
    }

    read(fd, buf, 255);
    j = 0;
    for (i = 0, buf_index=0; i < 255; i++)
    {
        if (buf[i] == ' ')
        {
            j++;
        }
        else
        {
            if (j == field_num)
            {
                current_pid[buf_index] = buf[i];
                buf_index++;
            }
            if (j > field_num)
                break;
        }
    }
    close(fd);
    return atoi(current_pid);
}
    
char *get_current_procname(int proc_num)
{   
    char *buf;
    char proc_file[256];
    
    buf = (char *)malloc(256);
    memset(buf, '\0', 256);

    memset(proc_file, '\0', 256);
    sprintf(proc_file, "/proc/%d/exe", proc_num);

    readlink(proc_file, buf, 256);
    return buf;
}
