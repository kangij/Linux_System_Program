#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int target = 0;

int temp=0;

void * addTarget(void * param)
{
	temp++;
	printf("\r\n thread1_temp[%x]",temp);
	sleep(10);
}

void * subtractTarget(void * param)
{
	temp++;
	printf("\r\n thread2_temp[%x]",temp);
	sleep(20);
}
 
int main()
{
  pthread_t add, sub;
  int param = 100;
  
  int add_id = pthread_create(&add, NULL, addTarget, &param);
  // error handling, 정상적으로 생성되면 0 반환
  if (add_id < 0)
  {
      perror("thread create error : ");
      exit(0);
  }
  
  int sub_id = pthread_create(&sub, NULL, subtractTarget, &param);
  if (sub_id < 0)
  {
      perror("thread create error : ");
      exit(0);
  }
 
  sleep(3000);
  printf("target : %d\n", target);
 
  return 0;
}
