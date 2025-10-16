/**
 *Noncompliant Code Example (atomic_bool)
 *This noncompliant code example declares a shared atomic_bool flag variable and provides a toggle_flag() method that negates the current value of flag
**/

#include <stdatomic.h>
#include <stdbool.h>
   
static atomic_bool flag = ATOMIC_VAR_INIT(false);
   
void init_flag(void) {
  atomic_init(&flag, false);
}
   
void toggle_flag(void) {
  bool temp_flag = atomic_load(&flag);
  temp_flag = !temp_flag;
  atomic_store(&flag, temp_flag);
}
     
bool get_flag(void) {
  return atomic_load(&flag);
}