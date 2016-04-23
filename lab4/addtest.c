#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

int opt_yield, opt_mutex, opt_slock, opt_sync;
pthread_mutex_t mutex;
volatile static int lock = 0;


void add(long long *pointer, long long value)
{
  if(opt_mutex)
    pthread_mutex_lock(&mutex);

  if(opt_slock)
    while(__sync_lock_test_and_set(&lock,1));
  
  long long sum = *pointer + value;
  if(opt_yield)
    pthread_yield();
  *pointer = sum;
  
  if(opt_mutex)
    pthread_mutex_unlock(&mutex);

  if(opt_slock)
    __sync_lock_release(&lock);

}

typedef struct
{
  int niter;
  long long* counter;
}threadData;

void *threadProcess(void* threadPtr)
{
  threadData *data = (threadData*) threadPtr;
  
  int i;
  if(!opt_sync)
    {
      for(i =0; i < data->niter; i++)
	add((data->counter), 1);
      
      for(i =0; i < data->niter; i++)
	add((data->counter), -1);
    }
  else
    {
      int old, sum;
      for (int i = 0; i < data->niter; i++)
	{
	  //if counter is the same as its old value, then counter = sum
	  //if we didnt write into counter then redo loop
	  do
	    {
	      old = *(data->counter); //old is current value of counter
	      sum = old + 1;          //add one to what you thought counter was
	      if (opt_yield)
		pthread_yield();
	    }while(__sync_val_compare_and_swap(data->counter, old, sum) != old);
	}
      
      for (int i = 0; i < data->niter; i++)
	{
	  do
	    {
	      old = *(data->counter);
	      sum = old - 1;
	      if (opt_yield)
		pthread_yield();
	    } while(__sync_val_compare_and_swap(data->counter, old, sum) != old);
	}
    }
}

int main(int argc, char* argv[])
{
  int nthreads=1, niter=1;
  opt_yield = 0;
  opt_mutex = 0;
  opt_slock = 0;
  opt_sync = 0;
  static struct option longopts[]= {
    {"iterations", 1, NULL, 'i'},
    {"threads", 1, NULL, 't'},
    {"yield", 1, NULL, 'y'},
    {"sync", 1, NULL, 's'},
    {0,0,0,0}
  };

  int longindex = 0 ,opt;
  while((opt=getopt_long(argc, argv, "", longopts, &longindex)) != -1)
    {
      if(opt == 'i')
	{
	  niter = atoi(optarg);
	}

      if(opt == 't')
	{
	  nthreads = atoi(optarg);
	}

      if(opt == 'y')
	{
	  opt_yield = atoi(optarg);
	}

      if(opt == 's')
	{
	  if(*optarg == 'm') //mutex
	    {
	      opt_mutex = 1;
	    }
	  if(*optarg == 's') //spin-lock
	    {
	      opt_slock = 1;
	    }
	  if(*optarg == 'c') //atomic __sync_
	    {
	      opt_sync = 1;
	    }
	}
    }
   
  long long counter = 0;
  struct timespec time_s;
  clock_gettime(CLOCK_MONOTONIC, &time_s);
  time_t start_sec = time_s.tv_sec;
  long start_nsec = time_s.tv_nsec;

  
  pthread_t threadArr[nthreads];
  threadData threadInfo[nthreads];
  int i, ret;
  for(i=0; i < nthreads; i++)
    {
      threadInfo[i].niter = niter;
      threadInfo[i].counter = &counter;
      
      ret = pthread_create(&threadArr[i], NULL, threadProcess, &threadInfo[i]);
      if(ret != 0)
	{
	  fprintf(stderr, "ERROR: Could Not Create Thread\n");
	  exit(1);
	}
    }

  for(i=0; i < nthreads; i++)
    {
      ret = pthread_join(threadArr[i], NULL);
      if(ret != 0)
	{
	  fprintf(stderr, "ERROR: Could Not Join Threads\n");
	  exit(1);
	}
    }
  struct timespec time_e;
  clock_gettime(CLOCK_MONOTONIC, &time_e);

  time_t total_sec = time_e.tv_sec - start_sec;
  long total_nsec = time_e.tv_nsec - start_nsec;

  long total_time = (long)total_sec*1000000000 + total_nsec;
  printf("%d threads x %d iterations x (add + subtract) = %d operations\n",
  	 nthreads, niter, 2*niter*nthreads); 
  
  printf("counter: %d\n",counter);
  if(counter != 0)
    fprintf(stderr, "ERROR: counter does not equal zero\n");
  
  printf("elasped time: %ld ns\n", total_time);
  
  printf("per operation: %ld ns\n", total_time/(2*niter*nthreads));
  
  // printf("%ld\t%ld\n", nthreads, total_time/(2*niter*nthreads));
}
