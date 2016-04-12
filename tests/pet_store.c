/** Author: Steve Hutchings
 *  
 *  Operating Systems Homework 3.A: Pet Store
 * 
 *  For this assignment we implemented a monitor using condition variables and
 *  a lock. This was tested by having a "pet store" where there were three types
 *  of animals. Animals had to be let out to "play" but only certain combinations
 *  of animals could play together as shown in the requirements below. The asserts
 *  in play() verfied that playing animals were always correct.
 * 
 *  Requirements:
 *  - Cats and dogs can't play together
 *  - Cats and birds can't play together
 *  - Dogs and birds CAN play together
 *  - Any number of cats can play together
 *  - Any number of dogs can play together
 *  - Any number of birds can play together
 *  - Between 0 and 99 for each animal must be supported
 *  - Run for ten seconds and then print out play time
 **/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

#define NUM_SECONDS 10

typedef enum {
  CAT,
  DOG,
  BIRD
} Animal;

typedef struct _thread_data_t {
  int threadId;
  int playTime;
  Animal type;
} thread_data_t;

volatile int numCats;
volatile int numDogs;
volatile int numBirds;
volatile int catsPlaying;
volatile int dogsPlaying;
volatile int birdsPlaying;
volatile int catPlayTime;
volatile int dogPlayTime;
volatile int birdPlayTime;

volatile time_t startTime, endTime;

// Monitor condition variables
pthread_cond_t cvCat;
pthread_cond_t cvDog;
pthread_cond_t cvBird;

// Monitor mutex
pthread_mutex_t lock;

// Monitor methods
int monitorInitialize(void) {
  int error = 0;

  error |= pthread_mutex_init(&lock, NULL);

  error |= pthread_cond_init(&cvCat, NULL);
  error |= pthread_cond_init(&cvDog, NULL);
  error |= pthread_cond_init(&cvBird, NULL);

  return error;
}

int cat_enter(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  while(birdsPlaying > 0 || dogsPlaying > 0) error = pthread_cond_wait(&cvCat, &lock);
  if(error) {
    printf("ERROR: pthread_cond_wait call failed. Exiting...\n");
    return -1;
  }

  catsPlaying++;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  return error;
}

int cat_exit(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  catsPlaying--;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_broadcast(&cvBird);
  if(error) {
    printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_broadcast(&cvDog);
  if(error) {
    printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
    return -1;
  }

  return error;
}

int dog_enter(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  while(catsPlaying > 0) error = pthread_cond_wait(&cvDog, &lock);
  if(error) {
    printf("ERROR: pthread_cond_wait call failed. Exiting...\n");
    return -1;
  }

  dogsPlaying++;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  return error;
}

int dog_exit(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  dogsPlaying--;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_broadcast(&cvCat);
  if(error) {
    printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
    return -1;
  }

  return error;
}

int bird_enter(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  while(catsPlaying > 0) error = pthread_cond_wait(&cvBird, &lock);
  if(error) {
    printf("ERROR: pthread_cond_wait call failed. Exiting...\n");
    return -1;
  }

  birdsPlaying++;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  return error;
}

int bird_exit(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  birdsPlaying--;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_broadcast(&cvCat);
  if(error) {
    printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
    return -1;
  }

  return error;
}

void play(void) {
  int i;
  for(i = 0; i < 10; i++) {
    assert(catsPlaying >= 0 && catsPlaying <= numCats);
    assert(dogsPlaying >= 0 && dogsPlaying <= numDogs);
    assert(birdsPlaying >= 0 && birdsPlaying <= numBirds);
    assert(catsPlaying == 0 || dogsPlaying == 0);
    assert(catsPlaying == 0 || birdsPlaying == 0);
  }
}

void *threadFunction(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
  data->playTime = 0;

  int error = 0;

  // Run for 10 seconds
  while(time(NULL) < endTime) { 

    // Play!
    if(data->type == CAT) {
      error = cat_enter();
      play();
      error = cat_exit();
     }
    else if(data->type == DOG) {
      error = dog_enter();
      play();
      error = dog_exit();
     }
    else if(data->type == BIRD) {
      error = bird_enter();
      play();
      error = bird_exit();
     }

    if(error) {
      printf("ERROR: monitor function call failed. See previous errors. Exiting...\n");
      pthread_exit((void *) &(error));
    }

    data->playTime++;
  }

  pthread_exit((void *)&(data->playTime));
}

int main(int argc, char *argv[]) {

  // Check basic command line args
  if(argc != 4) {
    printf("USAGE: %s num_cats num_dogs num_birds\n", argv[0]);
    return -1;
  }

  // Check valid arguments
  char* temp;
  numCats = strtol(argv[1], &temp, 10);
  if(numCats < 0 || numCats > 99 || temp == '\0' || temp == argv[1]) {
     printf("ERROR: Invalid number of cats. Valid range is 0 - 99. Exiting...\n");
    return -1;
  }
  numDogs = strtol(argv[2], &temp, 10);
  if(numDogs < 0 || numDogs > 99 || temp == '\0' || temp == argv[2]) {
    printf("ERROR: Invalid number of dogs. Valid range is 0 - 99. Exiting...\n");
    return -1;
  }
  numBirds = strtol(argv[3], &temp, 10);
  if(numBirds < 0 || numBirds > 99 || temp == '\0' || temp == argv[3]) {
    printf("ERROR: Invalid number of birds. Valid range is 0 - 99. Exiting...\n");
    return -1;
  }

  int numAnimals;
  int i;
  int error = 0;

  numAnimals = numCats + numDogs + numBirds;

  pthread_t threads[numAnimals];
  thread_data_t threadData[numAnimals];

  // Start a timer
  startTime = time(NULL);
  endTime = startTime + NUM_SECONDS;

  // Initialize our monitor
  error = monitorInitialize();
  if(error) {
    printf("ERROR: Monitor failed to initialize. Exiting...\n");
    return -1;
  }

  // Spawn all the animals
  for(i = 0; i < numAnimals; i++) {
    threadData[i].threadId = i;

    // Assign each thread an animal type
    if(i < numCats) {
      threadData[i].type = CAT;
    }
    else if((i - numCats) < numDogs) {
      threadData[i].type = DOG;
    }
    else {
      threadData[i].type = BIRD;
    }

    // Create the threads and check for success
    if(pthread_create(&threads[i], NULL, threadFunction, &threadData[i])) {
      printf("ERROR: pthread_create call %d failed!\n", i);
      return -1;
    }
  }

  void *individualPlayTime = 0;

  // Gather up all of the animals after 10 seconds
  for(i = 0; i < numAnimals; i++) {
    if(pthread_join(threads[i], &individualPlayTime)) {
      printf("ERROR: pthread_join call %d failed!\n", i);
      return -1;
    }

    if(i < numCats) {
      catPlayTime += *(int *)individualPlayTime;
    }
    else if((i - numCats) < numDogs) {
      dogPlayTime += *(int *)individualPlayTime;
    }
    else {
      birdPlayTime += *(int *)individualPlayTime;
    }
  }

  printf("cat play = %d, dog play = %d, bird play = %d\n", catPlayTime, dogPlayTime, birdPlayTime);

  return 0;
}
