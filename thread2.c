#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_main(void *argv[]);

int main(int argc, char *argv[])
{
	pthread_t thread_id;
	int thread_param = 5;
	void *thread_return;
	
	if (pthread_create(&thread_id, NULL, thread_main, (void *)&thread_param) != 0) {
		puts("pthread_create() error");
		return -1;
	}
	
	if (pthread_join(thread_id, &thread_return) != 0) {
		puts("pthread_join() error");
		return -1;
	{
	
	
	return 0;
}
