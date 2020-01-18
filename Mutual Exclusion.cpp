#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <thread>
#include <queue>
#include <unistd.h>
#include <time.h>
using namespace std;

void print();
void semInit();
void threadInit();
void reader ();
void writer ();
void producer ();
void consumer();


sem_t s, full, e, x, wsem;
int shared=0, readCount, SizeOfBuffer ,noThreads,temp=0, t1;

queue <int>buffer;


int main()
{

    print();
    semInit();
    threadInit();


    return 0;
}

void print()
{
  printf("Please enter the buffer size:\n");
    scanf("%d",&SizeOfBuffer);
    printf("Please enter the time : (note 1M = 1 sec)\n");
    scanf("%d",&t1);
    printf("Please enter the number of threads: \n");
    scanf("%d",&noThreads);
}

void semInit()
{
sem_init(&e, 0, SizeOfBuffer);
    sem_init(&full, 0, 0);
    sem_init(&s, 0, 1);
    sem_init(&x, 0, 1);
    sem_init(&wsem, 0, 1);
    }
    void threadInit()
    {
     std::thread mCounter[noThreads],mCollector, mMonitor ; // Array of writer threads

    for(int i=0; i<noThreads; i++)
    {
        mCounter[i] = std::thread(writer);
    }

    mMonitor = std::thread(reader);
    mCollector =std::thread(consumer);
    mMonitor.join();
    mCollector.join();

    for ( int i =0 ; i< noThreads ; i++)
        mCounter[i].join();


}


void reader ()
{
while(1){
    sem_wait(&x);
    readCount++;
    if(readCount == 1 )
        sem_wait(&wsem);
    sem_post(&x);
    temp = shared;
    printf("Monitor reads value  = %d \n", temp);
    shared = 0 ;
    sem_wait(&x);
    readCount--;

    if (readCount == 0)
        sem_post(&wsem);
    sem_post(&x);
    producer();
    usleep(t1);

}

}

void writer ()
{
    while (1)
    {
    sem_wait(&wsem);
    shared ++ ;
        printf("Counter thread reads value  = %d \n", shared);


    sem_post(&wsem);
    srand(time(0));
    usleep(rand() % 500000  +  1000);
    }

}

void producer ()
{

    if(buffer.size()== SizeOfBuffer )
    printf("Buffer is full \n ");
        sem_wait(&e);
        sem_wait(&s);
        buffer.push(temp);
        printf("Moniter inserted  %d  in  buffer\n", temp);
        sem_post(&s);
        sem_post(&full);


}

void consumer()
{
int oput;
    while (true)
    {
     if(buffer.size()== 0 )
        printf("Buffer is empty \n ");
        sem_wait(&full);
        sem_wait(&s);
        oput = buffer.front() ;
        buffer.pop();
        printf("Collector collects  %d  from  buffer\n", oput);
        sem_post(&s);
        sem_post(&e);
      srand(time(0));
    usleep(rand() % 500000  +  1000);
    }

}


