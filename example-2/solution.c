/**
 * Compliant Solution (using atomic_fetch_add)
 * This solution uses atomic_fetch_add which performs the entire
 * read-modify-write operation atomically.
 * 
 * atomic_fetch_add atomically:
 *   1. Reads the current value
 *   2. Adds the specified value to it
 *   3. Stores the result back
 *   4. Returns the original value
 * 
 * This is a single atomic operation that prevents race conditions
 * even with multiple threads.
 **/

#include <stdatomic.h>
#include <stdio.h>
#include <pthread.h>

static atomic_int counter = ATOMIC_VAR_INIT(0);

#define NUM_THREADS 5
#define INCREMENTS_PER_THREAD 10000

/**
 * Initializes the atomic counter to zero.
 * This function should be called before any thread operations begin.
 */
void init_counter(void) {
  atomic_init(&counter, 0);
}

/**
 * Increments the counter atomically using atomic_fetch_add.
 * This function performs a thread-safe increment operation.
 */
void increment_counter(void) {
  // Atomic operation: no race condition possible
  atomic_fetch_add(&counter, 1);
}

/**
 * Returns the current value of the atomic counter.
 * Uses atomic_load to ensure thread-safe reading.
 */
int get_counter(void) {
  return atomic_load(&counter);
}

/**
 * Thread function that repeatedly calls increment_counter.
 * Each thread performs INCREMENTS_PER_THREAD increment operations.
 */
void* thread_function(void* arg) {
  int thread_id = *(int*)arg;
  
  for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
    increment_counter();
  }
  
  printf("Thread %d finished\n", thread_id);
  return NULL;
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];
  
  init_counter();
  printf("Initial counter: %d\n", get_counter());
  printf("Starting %d threads, each incrementing %d times...\n\n", 
         NUM_THREADS, INCREMENTS_PER_THREAD);
  
  // Create threads
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i + 1;
    pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
  }
  
  // Wait for all threads to complete
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  int final_counter = get_counter();
  int expected_counter = NUM_THREADS * INCREMENTS_PER_THREAD;
  
  printf("\nExpected counter value: %d\n", expected_counter);
  printf("Actual counter value: %d\n", final_counter);
  
  if (final_counter == expected_counter) {
    printf("\n✓ SUCCESS! Counter value is correct.\n");
    printf("The atomic_fetch_add operation prevents race conditions.\n");
    printf("All %d increments were successfully applied.\n", expected_counter);
  } else {
    printf("\n⚠️  ERROR: Unexpected result!\n");
    printf("This should not happen with atomic_fetch_add.\n");
  }
  
  return 0;
}
