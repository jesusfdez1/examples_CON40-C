/**
 * Compliant Solution (using atomic XOR operator)
 * This solution uses the atomic compound assignment operator ^=
 * which performs the XOR operation atomically.
 * 
 * The ^= operator on an atomic variable is guaranteed to be atomic,
 * preventing race conditions when multiple threads access it.
 **/

#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
  
static atomic_bool flag = ATOMIC_VAR_INIT(false);
static atomic_int toggle_count = ATOMIC_VAR_INIT(0);

#define NUM_THREADS 10
#define TOGGLES_PER_THREAD 1000
  
void init_flag(void) {
  atomic_init(&flag, false);
  atomic_init(&toggle_count, 0);
}
  
void toggle_flag(void) {
  flag ^= 1;  // Atomic XOR operation
  atomic_fetch_add(&toggle_count, 1);
}
    
bool get_flag(void) {
  return flag;
}

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
  
  if (final_flag == (total_toggles % 2)) {
    printf("\n✓ SUCCESS! Flag value is correct.\n");
    printf("The atomic XOR operator prevents race conditions.\n");
  } else {
    printf("\n⚠️  ERROR: Unexpected result!\n");
  }
  
  return 0;
}