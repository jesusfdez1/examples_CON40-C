/**
 * Noncompliant Code Example (atomic_bool)
 * This noncompliant code example declares a shared atomic_bool flag variable 
 * and provides a toggle_flag() method that negates the current value of flag.
 * 
 * PROBLEM: The operation is split into three non-atomic steps:
 *   1. Read the flag value (atomic_load)
 *   2. Negate the value (!)
 *   3. Write the flag value (atomic_store)
 * 
 * This creates a race condition when multiple threads call toggle_flag()
 * simultaneously, potentially causing lost updates.
 **/

#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
   
static atomic_bool flag = ATOMIC_VAR_INIT(false);
static atomic_int toggle_count = ATOMIC_VAR_INIT(0);

#define NUM_THREADS 10
#define TOGGLES_PER_THREAD 1000

/**
 * Initializes the atomic flag to false and resets the toggle counter.
 * This function should be called before any thread operations begin.
 */
void init_flag(void) {
  atomic_init(&flag, false);
  atomic_init(&toggle_count, 0);
}

/**
 * Toggles the boolean flag value in a non-atomic way (UNSAFE).
 * This function demonstrates a race condition by splitting the operation
 * into separate load and store operations.
 */
void toggle_flag(void) {
  // RACE CONDITION: Between load and store, another thread can modify the flag
  bool temp_flag = atomic_load(&flag);
  temp_flag = !temp_flag;
  atomic_store(&flag, temp_flag);
  
  atomic_fetch_add(&toggle_count, 1);
}

/**
 * Returns the current value of the atomic flag.
 * Uses atomic_load to ensure thread-safe reading.
 */
bool get_flag(void) {
  return atomic_load(&flag);
}

/**
 * Thread function that repeatedly calls toggle_flag.
 * Each thread performs TOGGLES_PER_THREAD toggle operations.
 */
void* thread_function(void* arg) {
  (void)arg;  // Unused parameter
  
  for (int i = 0; i < TOGGLES_PER_THREAD; i++) {
    toggle_flag();
  }
  
  return NULL;
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  
  init_flag();
  printf("Initial flag: %d\n", get_flag());
  printf("Starting %d threads, each toggling %d times...\n\n", NUM_THREADS, TOGGLES_PER_THREAD);
  
  // Create threads
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, thread_function, NULL);
  }
  
  // Wait for all threads to complete
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  int total_toggles = atomic_load(&toggle_count);
  bool final_flag = get_flag();
  
  printf("Total toggles performed: %d\n", total_toggles);
  printf("Expected final flag value: %d (even number of toggles)\n", (total_toggles % 2));
  printf("Actual final flag value: %d\n", final_flag);
  
  if (final_flag != (total_toggles % 2)) {
    printf("\n⚠️  RACE CONDITION DETECTED! ⚠️\n");
    printf("The flag value is incorrect due to lost updates.\n");
    printf("This demonstrates the CON40-C violation.\n");
  } else {
    printf("\n✓ Flag value is correct (race condition may not have occurred this time).\n");
    printf("Run the program multiple times to observe the race condition.\n");
  }
  
  return 0;
}