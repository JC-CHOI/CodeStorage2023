#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100

void *thread_inc(void *arg);
void *thread_des(void *arg);

long long num;
pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    pthread_t thread_id[NUM_THREAD];
    int i;

    pthread_mutex_init(&mutex, NULL);

    for( i=0; i<NUM_THREAD; i++)
    {
        if( i%2 )
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }

    for( i=0; i<NUM_THREAD; i++)
        pthread_join(thread_id[i],NULL);

    printf("result : %lld\n",num);
    pthread_mutex_destroy(&mutex);

    return 0;
}
void *thread_inc(void *arg)
{
    int i;

    pthread_mutex_lock(&mutex);
    for( i=0; i<50000000; i++)
        num +=1;
    pthread_mutex_unlock(&mutex);
    // mutex lock / unlock 에는 생각보다 많은 시간이 소요됨.
    // 실행속도에 영향을 줄 것 같으면 호출을 최대한 줄이는게 나을듯
    return NULL;
}
void *thread_des(void *arg)
{
    int i;
    pthread_mutex_lock(&mutex);
    for( i=0; i<50000000; i++)
        num -= 1;
    pthread_mutex_unlock(&mutex);
    return NULL;
}