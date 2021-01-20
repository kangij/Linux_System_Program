#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct foo{
	int f_count;
	pthread_mutex_t f_lock;
};

struct foo *foo_alloc(void)
{
	struct foo *fp;
	fp = malloc(sizeof(struct foo));
	if(fp!=NULL)
	{
		fp->f_count=1;
		if(pthread_mutex_init(&fp->f_lock,NULL)!=0)
		{
			free(fp);
			return(NULL);
		}
	}
	return fp;
}

void foo_release(struct foo *fp)
{
	pthread_mutex_destroy(&fp->f_lock);
	free(fp);
}

void foo_hold_plus(struct foo *fp)
{
	if(!fp)
	 return ;
	
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
	
	printf("thread[%lu] , count[%x]",pthread_self(), fp->f_count);
}

void foo_hold_minus(struct foo *fp)
{
	if(!fp)
	 return ;
	
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count--;
	pthread_mutex_unlock(&fp->f_lock);
	
	printf("thread[%lu] , count[%x]",pthread_self(), fp->f_count);
	if(fp->f_count == 0) foo_release(fp);
}

void *th1(void *fp)
{
	//while(1)
	{
		foo_hold_plus(fp);
		sleep(1);
		foo_hold_plus(fp);
	}
	return ((void*)0);
}

void *th2(void *fp)
{
	//while(1)
	{
		foo_hold_minus(fp);
		sleep(1);
		foo_hold_minus(fp);
		sleep(1);
	}
	return ((void*)0);
}

void main(void)
{
	int err;
	pthread_t thread[2];
	pthread_mutex_t *fp;
	thread[0]=pthread_create(&thread[0],NULL,th1,fp);
	thread[1]=pthread_create(&thread[1],NULL,th2,fp);

	while(1)
	{	
		pause();
	}
	return ;
}
