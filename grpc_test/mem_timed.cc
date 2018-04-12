#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "threadpool.h"
#include "elapsed_time.h"

void memcpy_speed(unsigned long buf_size, int iters){
    //struct timeval start,  end;
    char * pbuff_1;
    char * pbuff_2;

    pbuff_1 = (char*)malloc(buf_size);
    pbuff_1[buf_size -1 ] = 0;
    pbuff_2 = (char*)malloc(buf_size);

    //gettimeofday(&start, NULL);
    for(int i = 0; i < iters; ++i){
        memcpy(pbuff_2, pbuff_1, buf_size);
    }   
    //gettimeofday(&end, NULL);
   // float time = ((end.tv_sec - \
    //    start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec))/1000.0;

    //printf("time:%.2f, %5.3f\n", time, ((buf_size*iters)/(1.024*1.024))/((end.tv_sec - \
        //start.tv_sec)*1000*1000+(end.tv_usec - start.tv_usec)));

    free(pbuff_1);
    free(pbuff_2);
}

using namespace paddle::framework;

int main(){
    //memcpy_speed(400 * 1048976, 1); 
    ElapsedTime time(false);
    const int loop_times= 32;
    const int mem_size=400 * 1048076;
    for(int i=0;i<loop_times;i++){
    Async([]{
            memcpy_speed(200 * 1048976, 1);
            });
    }
    ThreadPool::GetInstance()->Wait();

    double total_time = time.GetElapsed();

    printf("total:%d size:%.2f KB time:%.2f speed:%.2f MB/s\n",
            loop_times, (mem_size / 1024.0), total_time,
            mem_size * 1.0 * (loop_times) / (1024.0 * total_time));

    return 0;
}
