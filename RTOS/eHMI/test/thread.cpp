#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <iostream>

long long int n = 0;

void *thread_func(void* arg)
{
    for(int i = 0; i < 1000000; ++i)
        std::cout << i << std::endl;
    pthread_exit(nullptr);
}

int main()
{
    for(int i = 0; i < 10; ++i)
    {
        pthread_t pid;
		void *ret;
        pthread_create(&pid, nullptr, thread_func, nullptr); 
		//pthread_join(pid, &ret);
        //pthread_detach(pid);
    }
    sleep(10);
    return 0;
}

