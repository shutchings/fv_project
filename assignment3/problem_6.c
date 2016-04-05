/** Problem 6: Implement a pthreads program that creates a number of threads that repeatedly access a 
 *  critical section that is synchronized using Lamport's Bakery algorithm. It should:
 *  - Take two command line arguments: first, the number of threads, and second, the number of seconds
 *    to run for.
 *  - Before terminating it shoulds say how many times each thread entered the critical section.
 *  - Make sure starvation doesn't occur
 *  - Check for violations and mutual exclusion
 *  - Run for about 10 minutes without violations
 *  - Be restricted to a single cpu using this command: taskset -c 1 ./problem_1 5 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <math.h>

typedef struct _thread_data_t {
  int threadId;
} thread_data_t;

struct spin_lock_t {
  volatile int value;
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

// Acquire lock
void spin_lock(struct spin_lock_t *s) {
  while(atomic_cmpxchg(&s->value, 0, 1));
}

// Release lock
void spin_unlock(struct spin_lock_t *s) {
  s->value = 0;
}

volatile int totalPoints;
volatile int numPointsInCircle;
volatile int numThreads;
volatile time_t stopTime;
struct spin_lock_t lock;

void *threadFunction() {

  double x = 0;
  double y = 0;
  double c = 0;

  while(time(NULL) < stopTime) {

    x = -1 + 2*((float)rand()/RAND_MAX);
    y = -1 + 2*((float)rand()/RAND_MAX);

    c = sqrt((double)(x*x + y*y));

    if(c <= 1) {
      spin_lock(&lock);
      numPointsInCircle++;
      spin_unlock(&lock);
    }

    spin_lock(&lock);
    totalPoints++;
    spin_unlock(&lock);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  // Check basic command line args
  if(argc != 3) {
    fprintf(stderr, "USAGE: %s num_threads num_seconds\n", argv[0]);
    return -1;
  }

  // Check for valid thread number argument
  if(atoi(argv[1]) < 0) {
    fprintf(stderr, "ERROR: Number of threads must be non-negative\n");
    return -1;
  }

  // Check for valid seconds argument
  if(atoi(argv[2]) < 0) {
    fprintf(stderr, "ERROR: Number of seconds must be non-negative\n");
    return -1;
  }

  int i = 0;
  int numSeconds = 0;
  double pi = 0;

  numThreads = atoi(argv[1]);
  numSeconds = atoi(argv[2]);

  srand(time(NULL));

  pthread_t threads[numThreads];
  thread_data_t threadData[numThreads];

  stopTime = time(NULL) + numSeconds;
  totalPoints = 0;

  lock.value = 0;

  for(i = 0; i < numThreads; i++) {
    // Create threads
    if(pthread_create(&threads[i], NULL, threadFunction, &threadData[i])) {
      fprintf(stderr, "ERROR: pthread_create call %d failed!\n", i);
      return -1;
    }
  }

  // Wait for program time to finish
  for(i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  // Estimate pi
  pi = 4*((double)(numPointsInCircle))/(totalPoints);
  fprintf(stdout, "%f\n", pi);

  return 0;
}
