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

void *thr_fn1(void *arg)
{
	struct foo foo = {1,2,3,4};
	printf("th1:id is %lu\n", pthread_self());
	print_foo("thread1:\n", &foo);
	pthread_exit((void*)&foo); // struct 구조체를 넘김.
}

void *thr_fn2(void *arg)
{
	printf("thread 2:ID %lu\n",pthread_self());
	pthread_exit( (void*)0);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	struct foo *fp;

	pthread_create(&tid1, NULL, thr_fn1, NULL);
	pthread_join(tid1,(void*)fp);
	pthread_create(&tid2, NULL, thr_fn2, NULL);
	pthread_join(tid2,(void*)fp);

	sleep(1);
	print_foo("Paren...",fp);

	printf("Parent start!!\n");
}
