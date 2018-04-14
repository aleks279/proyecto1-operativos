#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/time.h>
#include "hash_table.h"

#define MEM 64000
#define QUANTUM_SEC 0
#define QUANTUM_NANO 10

int actual_thread = 0;
int thread_count = 0;
struct mutex* aux_mutex;
struct linked_list* rr_thread_list;
ucontext_t* wait_context;

void wait() {
  while(1) { printf("\n"); }
};

struct mutex {
  int in_use;
  void* arg;
};

struct thread {
  ucontext_t* context;
  void* function;
};

// thread methods
void my_thread_end(struct thread* thread) {
  linked_list_delete_data((void*)thread, rr_thread_list);
  free(thread);
}

int my_thread_join(struct thread* thread) {
  linked_list_insert((void*)thread, rr_thread_list);
}

int my_thread_detach(struct thread* thread) {
  linked_list_delete_data((void*)thread, rr_thread_list);
}

void my_thread_yield() {
  linked_list_shift_left(rr_thread_list);
  setcontext(((struct thread*)(rr_thread_list->head->data))->context);
}

struct thread* my_thread_create(void* function) {
  ucontext_t* new_context = malloc(sizeof(ucontext_t));
  getcontext(new_context);
  new_context->uc_link = wait_context;
  new_context->uc_stack.ss_sp = malloc(MEM);
  new_context->uc_stack.ss_size = MEM;
  new_context->uc_stack.ss_flags = 0;
  makecontext(new_context, (void*)function, 0);

  struct thread* thread = malloc(sizeof(struct thread));
  thread->context = new_context;
  return thread;
}

struct thread* my_thread_create_with_param(void *function, void* param, void* param2) {
  ucontext_t* new_context = malloc(sizeof(ucontext_t));
  getcontext(new_context);
  new_context->uc_link = wait_context;
  new_context->uc_stack.ss_sp = malloc(MEM);
  new_context->uc_stack.ss_size = MEM;
  new_context->uc_stack.ss_flags = 0;
  makecontext(new_context, (void*)function, 1, param, param2);

  struct thread* thread = malloc(sizeof(struct thread));
  thread->context = new_context;
  return thread;
}

// mutex methods
int my_mutex_init(void* arg) {
  if(hash_table_search((long)arg) != -1) return 1;

  struct mutex* new_mutex = malloc(sizeof(struct mutex));
  new_mutex->arg = arg;
  new_mutex->in_use = 0;
  hash_table_insert((long)arg, (long)new_mutex);

  return 1;
}

int my_mutex_unlock(void* mutex) {
  long aux = hash_table_search((long)mutex);

  if(aux == -1) return 1;

  aux_mutex = (struct mutex*)aux;
  aux_mutex->in_use = 0;

  return 1;
}

int my_mutex_destroy(void* mutex) {
  long aux = hash_table_search((long)mutex);

  if(aux == -1) return -1;

  aux_mutex = (struct mutex*)aux;
  free(aux_mutex);
  hash_table_delete((long)aux_mutex->arg);
}

int my_mutex_lock(void* mutex) {
  long aux = hash_table_search((long)mutex);

  if(aux == -1) return -1;

  aux_mutex = (struct mutex*) aux;

  if(aux_mutex->in_use == 1) {
    my_thread_yield();
    return 0;
  } else {
    aux_mutex->in_use = 1;
    return 1;
  }
  return -1;
}

// schedulers
void scheduler_init() {
  rr_thread_list = new_linked_list();

  wait_context = malloc(sizeof(ucontext_t));
  getcontext(wait_context);
  wait_context->uc_link = 0;
  wait_context->uc_stack.ss_sp = malloc(MEM);
  wait_context->uc_stack.ss_size = MEM;
  wait_context->uc_stack.ss_flags = 0;

  makecontext(wait_context, (void*)wait, 0);
}

void scheduler_start() {
  struct itimerval it;
  struct sigaction new_action, old_action;

  new_action.sa_handler = my_thread_yield;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;

  sigaction(SIGPROF, &new_action, &old_action);

  it.it_interval.tv_sec = QUANTUM_SEC;
  it.it_interval.tv_usec = QUANTUM_NANO;
  it.it_value.tv_sec = 1;
  it.it_value.tv_usec = 0;

  setitimer(ITIMER_PROF, &it, NULL);
}
