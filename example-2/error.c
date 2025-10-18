/**
 * Noncompliant Code Example (atomic_int counter)
 * This noncompliant code example uses an atomic_int counter but performs
 * a non-atomic read-modify-write operation.
 * 
 * PROBLEM: The increment operation is split into three non-atomic steps:
 *   1. Read the counter value (atomic_load)
 *   2. Increment the value (+1)
 *   3. Write the counter value (atomic_store)
 * 
 * This creates a race condition where multiple threads can read the same
 * value before any of them writes back, causing lost increments.
 * 
 * Example scenario:
 *   Thread 1: reads counter = 5
 *   Thread 2: reads counter = 5 (before Thread 1 writes)
 *   Thread 1: writes counter = 6
 *   Thread 2: writes counter = 6 (lost one increment!)
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
 * Increments the counter in a non-atomic way (UNSAFE).
 * This function demonstrates a race condition by splitting the increment
 * into separate load, add, and store operations.
 */
void increment_counter(void) {
  // RACE CONDITION: Non-atomic read-modify-write
  int temp = atomic_load(&counter);
  temp = temp + 1;
  atomic_store(&counter, temp);
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
  printf("Lost increments: %d\n", expected_counter - final_counter);
  
  if (final_counter != expected_counter) {
    printf("\n⚠️  RACE CONDITION DETECTED! ⚠️\n");
    printf("The counter value is incorrect due to lost updates.\n");
    printf("This demonstrates the CON40-C violation.\n");
    printf("Lost %.2f%% of increments due to race conditions.\n", 
           100.0 * (expected_counter - final_counter) / expected_counter);
  } else {
    printf("\n✓ Counter value is correct (race condition did not manifest this time).\n");
    printf("Run the program multiple times to observe the race condition.\n");
  }
  
  return 0;
}
