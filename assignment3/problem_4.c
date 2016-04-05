/** Problem 4: Write a spinlock using the following interface:
 *  void spin_lock (struct spin_lock_t *s);
 *  void spin_unllck (struct spin_lock_t *s);
 *  Use a compare and swap that is provided. Your spin_lock_t
 *  struct should have a single int that is held when 1 and free
 *  when the value is 0. Test your spinlock the same way you tested
 *  your multicore safe bakery with the mem fence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

typedef struct _thread_data_t {
  int threadId;
  int iterations;
} thread_data_t;

struct spin_lock_t {
  volatile int value; // This needs to be volatile for it to work
};

// Atomic compare and exchange provided
static inline int atomic_cmpxchg(volatile int *ptr, int old, int new) {
  int ret;
  asm volatile ("lock cmpxchgl %2,%1"
    : "=a" (ret), "+m" (*ptr)
    : "r" (new), "0" (old)
    : "memory");
  return ret;
}

// Acquire a lock
void spin_lock(struct spin_lock_t *s) {
  while(atomic_cmpxchg(&s->value, 0, 1));
}

// Release a lock
void spin_unlock(struct spin_lock_t *s) {
  s->value = 0;
}

time_t time(time_t *t);

volatile int * choosing;
volatile int * number;
volatile time_t startTime, endTime;
volatile int in_cs;
volatile int numThreads;

// Struct does not need to be volatile because we're accessing
// it with an atomic compare and swap. Its member variables do
// need to be volatile though.
struct spin_lock_t lock; 

void *threadFunction(void *arg) {
  int max = 0;
  int i = 0;

  thread_data_t *data = (thread_data_t *)arg;
  data->iterations = 0;

  // Execute for as long as specified time
  while(time(NULL) < endTime) {
   
    // Enter bakery...
    spin_lock(&lock);
    choosing[data->threadId] = 1;
    spin_unlock(&lock);

    // Find max ticket
    max = 0;
    for(i = 0; i < numThreads; i++) {
      if(number[i] > max) max = number[i];
    }
    spin_lock(&lock);
    number[data->threadId] = max + 1;
    spin_unlock(&lock);

    spin_lock(&lock);
    choosing[data->threadId] = 0;
    spin_unlock(&lock);

    for(i = 0; i < numThreads; i++) {
  while(choosing[i] == 1);

  while((number[i] != 0) && 
        ((number[i] < number[data->threadId]) ||
         ((number[i] == number[data->threadId]) && (i < data->threadId))));
    }

    // Critical section...
    data->iterations++;

    assert(in_cs == 0);
    in_cs++;
    assert(in_cs == 1);
    in_cs++;
    assert(in_cs == 2);
    in_cs++;
    assert(in_cs == 3);
    in_cs = 0;

    // Exit bakery...
    // No need for a lock here because only one thread can be in the critical section at a time
    number[data->threadId] = 0;
  }
  
  fprintf(stdout, "Thread %d entered the critical section %d times.\n", data->threadId, data->iterations);

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  // Check basic command line args
  if(argc != 3) {
    fprintf(stderr, "USAGE: %s num_threads num_seconds\n", argv[0]);
    return -1;
  }

  // Check for valid thread argument
  if(atoi(argv[1]) < 0) {
    fprintf(stderr, "ERROR: Number of threads must be non-negative\n");
    return -1;
  }

  // Check for valid run time in seconds 
  if(atoi(argv[1]) < 0) {
    fprintf(stderr, "ERROR: Number of seconds to run must be non-negative\n");
    return -1;
  }

  in_cs = 0;
  numThreads = atoi(argv[1]);
  int numSeconds = atoi(argv[2]);
  int i = 0;
  pthread_t threads[numThreads];
  thread_data_t threadData[numThreads];

  // Initialize our lock structures
  choosing = malloc(numThreads * sizeof(*choosing));
  number = malloc(numThreads * sizeof(*number));
  lock.value = 0;

  if(choosing == NULL || number == NULL) {
    fprintf(stderr, "ERROR: Malloc failed to allocate space for choosing and number arrays!\n");
    return -1;
  }

  for(i = 0; i < numThreads; i++) {
    choosing[i] = 0;
    number[i] = 0;
  }

  // Start a timer
  startTime = time(NULL);
  endTime = startTime + numSeconds;

  // Create the threads
  for(i = 0; i < numThreads; i++) {
    threadData[i].threadId = i;
    threadData[i].iterations = 0;
    if(pthread_create(&threads[i], NULL, threadFunction, &threadData[i])) {
      fprintf(stderr, "ERROR: pthread_create call %d failed!\n", i);
      return -1;
    }
  }

  // Wait for program time to finish
  for(i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
