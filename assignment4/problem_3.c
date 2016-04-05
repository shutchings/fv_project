/** Author: Steve Hutchings
 *  
 *  Problem 3: Multi threaded program that takes a name of a directory as an argument and
 *  the number of threads to perform the work. 
 *  Requirements:
 *  - For each file print name and checksum
 *  - Name and checksum separated by space and followed by newline
 *  - Checksum print in uppercase hex with leading zeros to 8 digits
 *  - CRC32 provided
 *  - Initial value of CRC state variable should be 0
 *  - Don't print directories and don't search through lower levels of directory
 *  - Print "ACCESS ERROR" for unaccessable files
 *  - Have proper command line argument handling
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>

static uint32_t crc32_tab[] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

uint32_t crc32(uint32_t crc, const void *buf, size_t size)
{
	const uint8_t *p;

	p = buf;
	crc = crc ^ ~0U;

	while (size--)
		crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

	return crc ^ ~0U;
}

typedef struct _thread_data_t {
  int threadId;
} thread_data_t;

// Globals
char ** pFullFileNames;
uint32_t * pChecksums;
int * pErrors;
int fileToWork;
int totalWorkDone;
int availableWork;
int numFiles;

// Monitor condition variables
pthread_cond_t workReceived;
pthread_cond_t workAvailable;
pthread_cond_t allWorkDone;

// Monitor mutex
pthread_mutex_t lock;

// Monitor functions
int monitorInitialize(void) {
  int error = 0;

  error = pthread_mutex_init(&lock, NULL);
  if(error) {
    printf("ERROR: pthread_mutex_init call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_init(&workReceived, NULL);
  if(error) {
    printf("ERROR: pthread_cond_init call failed. Exiting...\n");
    return -1;
  }
  error = pthread_cond_init(&workAvailable, NULL);
  if(error) {
    printf("ERROR: pthread_cond_init call failed. Exiting...\n");
    return -1;
  }
  error = pthread_cond_init(&allWorkDone, NULL);
  if(error) {
    printf("ERROR: pthread_cond_init call failed. Exiting...\n");
    return -1;
  }

  return error;
}

// Main thread uses this to assign work. It waits until previously
// assigned work has been picked up. Signals to waiting threads
// that work is ready to be picked up.
int assignWork(int nextFile) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  while(availableWork != 0) error = pthread_cond_wait(&workReceived, &lock);
  if(error) {
    printf("ERROR: pthread_cond_wait call failed. Exiting...\n");
    return -1;
  }

  fileToWork = nextFile;
  availableWork = 1;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_broadcast(&workAvailable);
  if(error) {
    printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
    return -1;
  }

  return error;
}

// Threads ask for work and wait till the available work variable
// is set. This signals to the main thread that it can assign more
// work
int askForWork(void) {
  int temp;
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  while(availableWork == 0) error = pthread_cond_wait(&workAvailable, &lock);
  if(error) {
    printf("ERROR: pthread_cond_wait call failed. Exiting...\n");
    return -1;
  }

  temp = fileToWork;
  availableWork = 0;

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  error = pthread_cond_broadcast(&workReceived);
  if(error) {
    printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
    return -1;
  }

  return temp;
}

// Threads update the total work done and signal if all of the work
// has been finished so the main thread can start printing.
int workFinished(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  totalWorkDone++;
  
  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  if(totalWorkDone == numFiles) {
    error = pthread_cond_broadcast(&allWorkDone);
    if(error) {
      printf("ERROR: pthread_cond_broadcast call failed. Exiting...\n");
      return -1;
    }
  }

  return error;
}

// Once all the work has been assigned, the main thread must wait
// until all the threads have finished doing the work. 
int waitForAllWorkToFinish(void) {
  int error = 0;

  error = pthread_mutex_lock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_lock call failed. Exiting...\n");
    return -1;
  }

  while(totalWorkDone < numFiles) error = pthread_cond_wait(&allWorkDone, &lock);
  if(error) {
    printf("ERROR: pthread_cond_wait call failed. Exiting...\n");
    return -1;
  }

  error = pthread_mutex_unlock(&lock);
  if(error) {
    printf("ERROR: pthread_mutex_unlock call failed. Exiting...\n");
    return -1;
  }

  return error;
}

void *threadFunction(void *arg) {
  int workingFileNumber = 0;
  FILE * currentFile;
  int fileLength;
  uint32_t crc;
  int error = 0;

  while(totalWorkDone < numFiles) {
    
    workingFileNumber = askForWork();
    if(workingFileNumber < 0) {
      printf("ERROR: askForWork monitor function call failed. See previous errors. Exiting...\n");
      pthread_exit(NULL);
    }

    // Do work on the file
    currentFile = fopen(pFullFileNames[workingFileNumber], "r");
    if(currentFile == NULL) { // Check if file opened correctly
      pErrors[workingFileNumber] = 1; // Signal an error
      if(fclose(currentFile)) {
	printf("ERROR: fclose failed. Exiting...\n");

	pthread_exit(NULL);
      }
    }
    else {
      // Find size of file
      fseek(currentFile, 0L, SEEK_END);
      fileLength = ftell(currentFile);
      fseek(currentFile, 0L, SEEK_SET);

      // If the file size is 0, record 0
      if(fileLength == 0) {
	pChecksums[workingFileNumber] = 0;
	if(fclose(currentFile)) {
	  printf("ERROR: fclose failed. Exiting...\n");

	  pthread_exit(NULL);
	}
      }
      else {
	char * fileContents;
	crc = 0;

	// Put file contents into a buffer for CRC
	fileContents = (char *) malloc(sizeof(char) * fileLength);
	if(fileContents == NULL) {
	  printf("ERROR: malloc for fileContents failed!\n");
	  pErrors[workingFileNumber] = 1;
	}
	else {
	  error = fread(fileContents, sizeof(char), fileLength, currentFile);
	  if(error == 0) {
	    printf("ERROR: fread call failed. Exiting...\n");
	    pthread_exit(NULL);
	  }
	  crc = crc32(0, fileContents, fileLength);
	  pChecksums[workingFileNumber] = crc;

	  free(fileContents); // We're done with the buffer
	}
      }
    }

    error = workFinished(); // Signal for more work
    if(error) {
      printf("ERROR: workFinished monitor function call failed. Exiting...\n");
      
      pthread_exit(NULL);
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  int numThreads;

  // Check basic command line args
  if(argc != 3) {
    printf("USAGE: %s directory_name num_threads\n", argv[0]);
    return -1;
  }

  // Check valid argument
  char * temp;
  numThreads = strtol(argv[2], &temp, 10);
  if(numThreads < 0 || numThreads > 99 || temp == '\0' || temp == argv[2]) {
    printf("ERROR: Invalid number of threads. Valid range is 0 - 99\n");
    return -1;
  }

  struct dirent **fileList;  // Result from scandir, contains everything in the directory

  int numDirItems; // Total items in directory
  int n; 
  int i;
  int totalWorkAssigned = 0;
  int error = 0;

  pthread_t threads[numThreads];
  thread_data_t threadData[numThreads];

  // Scan through the directory and sort names into fileList
  numDirItems = scandir(argv[1], &fileList, NULL, alphasort);

  // Check to see if directory exists and scandir succeeded
  if(numDirItems < 0) {
    printf("ERROR: Error scanning directory! Please check directory name. Exiting...\n");
    return -1;
  }
  
  // Find only normal files to do operations on
  n = 0;
  numFiles = 0;
  while(n < numDirItems) {
    if(fileList[n]->d_type == DT_REG) {
      numFiles++;  
    }
    n++;
  }

  // Create our structure to hold our full filenames and crc
  char * fullFileNames[numFiles];
  char * shortFileNames[numFiles];
  uint32_t crcs[numFiles];
  int errors[numFiles];

  // Initilize errors and crc arrays
  for(i = 0; i < numFiles; i++) {
    crcs[i] = 0;
    errors[i] = 0;
  }

  // Set up global pointers
  pFullFileNames = fullFileNames;
  pChecksums = crcs;
  pErrors = errors;

  // Check to see if directory exists and scandir succeeded
  if(numFiles < 0) {
    printf("ERROR: Directory does not exist! Exiting...\n");
    return -1;
  }
  else { // Process the files in the directory
    
    n = 0;
    i = 0;
    while(n < numDirItems) {

      // Only process regular files
      if(fileList[n]->d_type == DT_REG) {

	// Create a short file name for printing later
	char* shortName = (char*) malloc(sizeof(char) * (strlen(argv[1]) + strlen(fileList[n]->d_name)));
	if(shortName == NULL) {
	  printf("ERROR: malloc call failed. Exiting...\n");
	  return -1;
	}

	memset(shortName, '\0', sizeof(shortName));
	shortName = fileList[n]->d_name;
	shortFileNames[i] = shortName;

	// Create full file name
	char* fileName = (char*) malloc(sizeof(char) * (strlen(argv[1]) + strlen(fileList[n]->d_name) + 2));
	if(fileName == NULL) {
	  printf("ERROR: malloc call failed. Exiting...\n");
	  return -1;
	}

	memset(fileName, '\0', sizeof(fileName));
	fileName = strcpy(fileName, argv[1]);
	
	// Check for trailing "/"
	if(fileName[strlen(fileName + 1)] != '/') {
	  strcat(fileName, "/");
	}

	// Add file name
	fileName = strcat(fileName, fileList[n]->d_name);

	// Store file name in array
	fullFileNames[i] = fileName;
	crcs[i] = 0;

	i++; // Next file
      }
      n++; // Next directory item
    }

    // Make our threads and let them go to work
    for(i = 0; i < numThreads; i++) {
      threadData[i].threadId = i;

      if(pthread_create(&threads[i], NULL, threadFunction, &threadData[i])) {
	printf("ERROR: pthread_create call %d failed! Exiting...\n", i);
	return -1;
      }
    }

    // Wait for a thread to ask for work
    while(totalWorkAssigned < numFiles) {
      error = assignWork(totalWorkAssigned);
      if(error) {
	printf("ERROR: assignWork monitor function call failed. Exiting\n");
	return -1;
      }
      totalWorkAssigned++;
    }

    // Wait for threads to finish up
    error = waitForAllWorkToFinish();
    if(error) {
      printf("ERROR: waitForAllWorkToFinish monitor function call failed. Exiting\n");
      return -1;
    }
  }

  // Print out filenames and results
  for(i = 0; i < numFiles; i++) {
    if(errors[i] == 1) { // Error in file read
      printf("%s ACCESS ERROR\n", shortFileNames[i]);
    }
    else if (errors[i] == 2) { // File size was 0
      printf("%s ACCESS ERROR\n", shortFileNames[i]);
    }
    else { // File processed successfully
      printf("%s %08X\n", shortFileNames[i], crcs[i]);
    }
  }

  return 0;
}
