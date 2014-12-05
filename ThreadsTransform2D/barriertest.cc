#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

pthread_barrier_t global_barrier;

void *mythr(void *arg)
{
    int n = (int)arg;
    printf("thr #%d, enter, time = %d\n", n, time(NULL));
    sleep(n);
    if ( pthread_barrier_wait(&global_barrier) == PTHREAD_BARRIER_SERIAL_THREAD )
            printf("thr #%d, SERIAL_THREAD, leave, time = %d\n", n, time(NULL));
    else
            printf("thr #%d, leave, time = %d\n", n, time(NULL));
    return (void *)0;
}

int main()
{
    pthread_t pid1, pid2, pid3;
    pthread_barrier_init(&global_barrier, NULL, 3);

    pthread_create(&pid1, NULL, mythr, (void *)1);
    pthread_create(&pid2, NULL, mythr, (void *)3);
    pthread_create(&pid3, NULL, mythr, (void *)5);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    pthread_join(pid3, NULL);
    return 0;
}