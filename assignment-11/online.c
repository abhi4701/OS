// Author: Lucas Wilson // Email: lkwilson96@gmail.com
// Created for CS370 Honors Assignment
/**
 * This program provides a solution to the first reader-writer problem and will
 * exit with a value of 1 on error, 0 otherwise. Appropriate error messages
 * will be printed.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
//#include <error.h> // commented for Mac OS X support
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <pthread.h>

//******************************************************************************
/* READER-WRITER DEFINITIONS */
/**
 * The basic usage: start_thread(READER) or start_thread(WRITER). Don't call
 * any of the other functions yourself. They are separated to demonstrate the
 * different components to this problem.
 */

/*
 * Starts a reader or writer thread. Pass one of the READER/WRITER aliases as
 * type to specify which.
 */
#define READER 1
#define WRITER 2
/* CONFIGURABLE: Comment or uncomment */
#define THREAD_VERBOSE // if defined, it will log when threads start/end
pthread_t start_thread(unsigned type);

/* The locks used by the solution. See functions reader and writer. */
pthread_mutex_t rw;         // the writers lock
pthread_mutex_t r;          // the rc resource lock
int rc = 0;                 // reader count
/*
 * The reader/writer thread routines. Takes one arg of type int* which stores
 * the display id, used for output. Normally, I would just print the thread ID,
 * but it was a very large number (unsigned long) and not very easy to
 * differentiate between when dealing with 20+ total threads. The allocated
 * int* is freed at the end of the routine.
 */
void *reader(void*);        // the reader thread routine
void *writer(void*);        // the writer thread routine
/*
 * I wanted to separate the actual reading/writing of the file from the thread
 * routines themselves so that these specifics could be manually implemented
 * later, were this to be turned into a library.
 */
char *filename; // the file for the readers/writers to read/write to
#define HASH_SIZE 1000      // used to calculate hash of file
void read_file(int id);     // function to define how the reader reads
/* CONFIGURABLE: The writers write this many random bytes to the file */
#define MAX_WRITE_SIZE 1024 // the write_file writes this many random chars
void write_file(int id);    // function to define how the writer writes

//******************************************************************************
/* IMPLEMENTATION SECTION */
pthread_t start_thread(unsigned type) {
    // counts readers/writers and assigns them a more readable id than tid
    static unsigned rc = 0;
    static unsigned wc = 0;

    // build thread
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr); // defaults are fine

    // start thread
    int *id = malloc(sizeof(*id));
    switch (type) {
    case READER:
        *id = ++rc;
        pthread_create(&tid, &attr, reader, id);
        break;
    case WRITER:
        *id = ++wc;
        pthread_create(&tid, &attr, writer, id);
        break;
    // Assume function call is valid..
    }
    return tid;
}

void *reader(void *arg) {
    int id = *((int*)arg);
#ifdef THREAD_VERBOSE
    printf("[reader: %d] Thread started.\n", id);
#endif

    /* IMPLEMENTATION
     * Readers lock r, so all but one gets through, and the rest block. For the
     * one that gets through, there are two cases. Since rc starts at zero and
     * every increment is followed by a decrement, rc>=0
     *
     * Case 1: rc==0
     * If rc is 0, there are no readers reading, so the current reader is the
     * first. It then locks rw thus preventing writers from entering. If a
     * writer already locked it, the reader will block here until the writer is
     * done. While it's blocked any other readers are still blocked on r above
     * ensuring that no other readers enter while the writer is writing. When
     * the lock is released and the first reader is able to lock rw, it unlocks
     * r and starts reading. Any immediate subsequent readers fall into case 2
     * now that rc has been incremented.
     *
     * Case 2: rc>0
     * If rc is non zero, the readers are able to lock r, increment rc, and
     * unlock r for the next reader. They don't attempt to lock rw since rc==0
     * is false and therefore will never block.
     */
    pthread_mutex_lock(&r);
    if (rc++==0) pthread_mutex_lock(&rw);
    pthread_mutex_unlock(&r);

    read_file(id);

    /*
     * Since readers are reading, rw is locked, so no writers are writing. We
     * can therefore ignore them since they are all blocked. There are then two
     * cases of interest. At this point, rc has been incremented at least once,
     * so rc>=1
     *
     * Case 1: rc==1
     * If rc==1, then it's the only reader reading. Since r is locked, no
     * readers can enter. rc is decremented before boolean evaluation so the if
     * statement is true and rw is unlocked. 
     *
     * Case 2: rc>1
     * If rc is non zero, rc is decremented. Done enough times, case 1 is
     * induced.
     */
    pthread_mutex_lock(&r);
    if (--rc==0) pthread_mutex_unlock(&rw);
    pthread_mutex_unlock(&r);

#ifdef THREAD_VERBOSE
    printf("[reader: %d] Thread exiting.\n", id);
#endif
    free(arg);
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int id = *((int*)arg);
#ifdef THREAD_VERBOSE
    printf("[writer: %d] Thread started.\n", id);
#endif

    /* Implementation */
    /*
     * First writer locks rw. If it is already locked because of a reader,
     * writer blocks until a reader unlocks it. All subsequent writers block
     * here until rw is unlocked.
     */
    pthread_mutex_lock(&rw);
    write_file(id);
    /*
     * Unlocks rw signaling that writing is complete, at which point a reader
     * or writer (either when one shows up later or if one is already waiting)
     * will lock rw and begin reading/writing.
     */
    pthread_mutex_unlock(&rw);

#ifdef THREAD_VERBOSE
    printf("[writer: %d] Thread exiting.\n", id);
#endif
    free(arg);
    pthread_exit(NULL);
}

void read_file(int id) {
    printf("[reader: %d] Started reading.\n", id);
    /* Open the file */
    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        perror("Unable to open the file for reading");
        exit(1);
    }
    /* Read file and calculate hash */
    int hash = 0;
    for (int c = fgetc(fd); c!=EOF; c=fgetc(fd)) {
        /* Check for read error */
        if (ferror(fd)) {
            perror("Error while reading the file");
            exit(1);
        }
        hash += c;
    }
    hash %= HASH_SIZE;
    /* Close the file */
    if (fclose(fd)!=0) {
        perror("Unable to close the file opened by reader");
        exit(1);
    }
    printf("[reader: %d] Finished reading file with hash: %d\n", id, hash);
}

void write_file(int id) {
    printf("[writer: %d] Started writing.\n", id);
    // Use w+ to overwrite file every write. If you don't, hash will be
    // inaccurate. Use r+ to do what a real writer would likely do.
    /* Open the file */
    FILE *fd = fopen(filename, "w+");
    if (fd == NULL) {
        perror("Unable to open the file for writing");
        exit(1);
    }
    /* Write to the file and calculate hash */
    int hash = 0;
    for (unsigned i = 0; i < MAX_WRITE_SIZE; i++) {
        unsigned char c = rand();
        fputc((char)c, fd);
        /* Check for write error */
        if (ferror(fd)) {
            perror("Error while writing to the file");
            exit(1);
        }
        hash += (int)c;
    }
    hash %= HASH_SIZE;
    /* Close the file */
    if (fclose(fd)!=0) {
        perror("Unable to close the file opened by writer");
        exit(1);
    }
    printf("[writer: %d] Write to file with hash: %d\n", id, hash);
    printf("[writer: %d] Done writing.\n", id);
}

//******************************************************************************
/* TEST FUNCTIONS */
// These functions simulate some scenario. They should all start threads and join each of them.
// The reason for joining is because the filename is passed as an argument to
// the executable and therefore lives on the main stack. If the main thread
// ends, that might get overwritten (but I'm not sure.)
/*
 * Simple demonstration of starting and joining the reader and writer thread.
 */
void simple_test() {
    printf("Starting reader thread...\n");
    pthread_t rtid = start_thread(READER);
    printf("Started reader thread.\n");

    printf("Starting writer thread...\n");
    pthread_t wtid = start_thread(WRITER);
    printf("Started writer thread.\n");
    
    printf("Joining reader thread...\n");
    pthread_join(rtid, NULL);
    printf("Joined reader thread.\n");

    printf("Joining writer thread...\n");
    pthread_join(wtid, NULL);
    printf("Joined writer thread.\n");
}

/*
 * This will start r readers and w writers. It will start all writers and then
 * start all readers.
 */
void startrw(unsigned r, unsigned w) {
    pthread_t threads[r+w];
    printf("Starting %u writers.\n", w);
    for (unsigned i = 0; i < w; i++) threads[r+i] = start_thread(WRITER);
    printf("Starting %u readers.\n", r);
    for (unsigned i = 0; i < r; i++) threads[i] = start_thread(READER);
    for (unsigned i = 0; i < r+w; i++) pthread_join(threads[i], NULL);
    printf("All readers and writers' threads are done executing.\n");
}

/*
 * This will start x readers and x writers. It will alternate between starting
 * writer and reader threads starting with the writer first.
 */
void startx(unsigned x) {
    pthread_t threads[x*2];
    printf("Starting %u readers and writers total.\n", x*2);
    for (unsigned i = 0; i < x; i++) {
        threads[i*2+1] = start_thread(WRITER);
        threads[i*2] = start_thread(READER);
    }
    for (unsigned i = 0; i < x*2; i++) pthread_join(threads[i], NULL);
    printf("All readers and writers' threads are done executing.\n");
}

/*
 * This will demonstrate the starving of writers.
 */
void starve_writer() {
    printf("Starting 10 readers.\n");
    pthread_t threads[100];
    for (int i = 0; i < 10; i++) threads[i] = start_thread(READER);
    printf("Starting writer.\n");
    threads[10] = start_thread(WRITER);
    printf("Starting remaining readers.\n");
    for (int i = 11; i < 100; i++) threads[i] = start_thread(READER);
    for (unsigned i = 0; i < 100; i++) pthread_join(threads[i], NULL);
}

//******************************************************************************
/* MAIN */
int main(int argc, char *argv[]) {
    // rand() used by write_file
    srand(time(NULL));
    // Buffers make the output stream of multithreading inaccurate.
    setbuf(stdout, NULL);
    // Initialize the mutex locks.
    pthread_mutex_init(&r, NULL);
    pthread_mutex_init(&rw, NULL);

    /* Check args */
    if (argc != 2) { // Incorrect number of arguments
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        return 1;
    } else { // File doesn't exist
        filename = argv[1];
        if (access(filename, R_OK|W_OK) == -1) {
            perror("Please provide existing file with rw permissions");
            return 1;
        }
    }

    /* Start implementation, call test function */
    /* CONFIGURABLE: Call any of the test functions above here  */
    startx(10);         // start 10 readers/writers
    starve_writer();    // prove that writer can be starved
}