#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct foo
{
	int a,b,c,d;
};

void print_foo(const char *s, const struct foo *fp)
{

	printf("%s",s);
	printf("struct at 0x %x\n", (unsigned)fp);
	printf("foo.a =%d\n", fp->a);
	printf("foo.b =%d\n", fp->b);
	printf("foo.c =%d\n", fp->c);
	printf("foo.d =%d\n", fp->d);
}

void cleanup(void *arg)
{
	printf("cleanup : %s\n", (char *)arg);
}

void *thr_fn1(void *arg)
{
	struct foo foo = {1,2,3,4};
	printf("th1:id is %lu\n", pthread_self());
	print_foo("thread1:\n", &foo);
	pthread_cleanup_push(cleanup,"thread1 first");
	pthread_cleanup_push(cleanup,"thread1 two");
	if(arg)
	 return ((void *)1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
	return ((void*)1);
}

void *thr_fn2(void *arg)
{
	printf("thread 2:ID %lu\n",pthread_self());
	pthread_cleanup_push(cleanup,"thread2 first");
	pthread_cleanup_push(cleanup,"thread2 two");
	if(arg)
		pthread_exit( (void*)2);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
	pthread_exit((void*)2);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	struct foo *fp;
	void *tret;

	pthread_create(&tid1, NULL, thr_fn1, NULL);
	pthread_join(tid1,(void*)tret);
	pthread_create(&tid2, NULL, thr_fn2, NULL);
#if 0
	err=pthread_join(tid2,(void*)tret);
	if(err)
	{
		printf(" cant joint thread 22");
		pthread_detach(tid2);
	}
#else

		pthread_detach(tid2);
#endif

	sleep(1);
	print_foo("Paren...",fp);

	printf("Parent start!!\n");
	exit(1);
}
