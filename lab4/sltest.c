#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <ctype.h>

#include "SortedList.h"

int opt_yield, opt_mutex, opt_slock, opt_sync;
pthread_mutex_t mutex;
volatile static int lock = 0;
SortedList_t list;
SortedListElement_t* all_elements;
char** keys;

typedef struct
{
  int niter;
  int start;
}threadData;

void *threadProcess(void* threadPtr)
{
  threadData *data = (threadData*) threadPtr;
  int ret =0;
  for(int i =data->start; i < (data->niter + data->start); i++)
        SortedList_insert(&list, &all_elements[i]);

  SortedList_length(&list);

  for(int i =data->start; i < (data->niter + data->start); i++)
    {
      SortedListElement_t *a = SortedList_lookup(&list, keys[i]);

      if(a ==NULL)
	{
	  fprintf(stderr, "ERROR: couldn't find added element\n");
	  exit(1);
	}

      //ret=SortedList_delete(a);

        if (ret != 0) {
      fprintf(stderr, "ERROR: corrupt pointers for delete\n");
      	exit(1);
      }
    }
}

int main(int argc, char* argv[])
{

  int nthreads=1, niter=1, i;
  opt_yield = 0;
  opt_mutex = 0;
  opt_slock = 0;
  opt_sync = 0;
  static struct option longopts[]= {
    {"iterations", 1, NULL, 'i'},
    {"threads", 1, NULL, 't'},
    {"yield", 1, NULL, 'y'},
    {0,0,0,0}
  };

  int longindex = 0 ,opt;

  while((opt=getopt_long(argc, argv, "", longopts, &longindex)) != -1)
    {
      //iterations
      if(opt == 'i')
	{
	  niter = atoi(optarg);
	}

      //threads
      if(opt == 't')
	{
	  nthreads = atoi(optarg);
	}

      //yield
      if(opt == 'y' && optarg != NULL)
	{
	  for (i = 0; i < strlen(optarg); i++)
	    {
	      if (optarg[i] == 'i')
		opt_yield |= INSERT_YIELD;
	      else if (optarg[i] == 'd')
		opt_yield |= DELETE_YIELD;
	      else if (optarg[i] == 's')
		opt_yield |= SEARCH_YIELD;
	      else
		{
		  fprintf(stderr, "ERROR: invalid yield option\n");
		  exit(1);
		}
	    }
	}
    }
 
  list.prev = &list;
  list.next = &list;
  list.key = NULL;

  
  //creat random nodes
  all_elements = malloc(niter*nthreads*sizeof(SortedListElement_t));
  if (all_elements == NULL) {
    fprintf(stderr, "ERROR: unable to allocate memory\n");
    exit(1);
  }
  keys = malloc(niter*nthreads*sizeof(char *));
  if (keys == NULL) {
    fprintf(stderr, "ERROR: unable to allocate memory\n");
    exit(1);
  }

  //int lengths[niter*nthreads];
  for(i =0; i < (niter*nthreads); i++)
    {
      int string_length = rand()%101+1;
      keys[i] = malloc(string_length*sizeof(char));
      if (keys[i] == NULL) {
	fprintf(stderr, "ERROR: unable to allocate memory\n");
	exit(1);
      }
      for( int j = 0; j < string_length-1; ++j)
	keys[i][j] = '0' + rand()%72;
      
      // append null byte
      keys[i][string_length-1] = '\0';

      // Save as element
      all_elements[i].key = keys[i];
      /*
      lengths[i] = rand()%101+1; //random length key
      char random[lengths[i]];

      for(int n = 0; n < lengths[i]-1; n++)
	random[n] = '0' + rand()%72; //72 ascii characters
      random[lengths[i]-1] = '\0';

      keys[i] = random;
      all_elements[i].key = random;
      */
    }

  //start time
  struct timespec time_s;
  clock_gettime(CLOCK_MONOTONIC, &time_s);
  time_t start_sec = time_s.tv_sec;
  long start_nsec = time_s.tv_nsec;
  
  //start threads
  //  pthread_t threadArr[nthreads];
  pthread_t* threadArr = malloc(nthreads*sizeof(pthread_t));
  if (threadArr == NULL) { fprintf(stderr, "ERROR: malloc error\n"); exit(1); }
  threadData threadInfo[nthreads];
  int pos =0,range=0, ret;
  for(i=0; i < nthreads; i++)
    {
      threadInfo[i].niter = niter;
      threadInfo[i].start = range;
      range+=niter;
      
      ret = pthread_create(&threadArr[i], NULL, threadProcess, &threadInfo[i]);
      if(ret != 0)
	{
	  fprintf(stderr, "ERROR: Could Not Create Thread\n");
	  exit(1);
	}
      
    }
  //  /*
  //join threads
  for(i=0; i < nthreads; i++)
    {
      
      ret = pthread_join(threadArr[i], NULL);
      if(ret != 0)
	{
	  fprintf(stderr, "ERROR: Could Not Join Threads\n");
	  exit(1);
	}
      
    }
  //*/
  struct timespec time_e;
  clock_gettime(CLOCK_MONOTONIC, &time_e);

  time_t total_sec = time_e.tv_sec - start_sec;
  long total_nsec = time_e.tv_nsec - start_nsec;

  long total_time = (long)total_sec*1000000000 + total_nsec;
  long operations = 50*nthreads*niter*nthreads*niter;
  printf("%d threads x %d iterations x (ins + lookup/del) x (50 avg len) = %d operations\n",
	 nthreads, niter, operations);

  //printf("counter: %d\n",counter);
  if(SortedList_length(&list) != 0)
    fprintf(stderr, "ERROR: list does not equal zero\n");

  printf("elasped time: %ld ns\n", total_time);

  printf("per operation: %ld ns\n", total_time/operations);

  //printf("%ld\t%ld\n", nthreads, total_time/(2*niter*nthreads));
//*/
}
