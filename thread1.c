#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void *thread_main(void *arg);

int main(int argc, char *argv[])
{
	pthread_t thread_id;
	int thread_param = 5;
	
	if (pthread_create(&thread_id, NULL, thread_main, (void *) &thread_param) != 0) {
		puts("pthread_create() error");
		return -1;
	}
	sleep(10);
	puts("end of main");
	
	return 0;
}

void *thread_main(void *arg)
{
	int cnt = *((int *)arg);
	for (int i = 0; i < cnt; ++i) {
		sleep(1);
		puts("running thread");
	}
	return NULL;
}
