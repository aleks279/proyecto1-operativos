#ifndef __MTHREAD_H
#define __MTHREAD_H
#include <ucontext.h>
 
typedef struct {
	int id;      // id del hilo
	ucontext_t uc; // contexto
	void *retval;  // guarda lo que retorna el hilo 
} mthread;

typedef struct node {
	struct node *next;
	mthread *data;
} node; 

typedef struct queue {
	struct node *front; // estructura de cola para implementar el algoritmo round robin 
	struct node *rear;
	int count;
} queue;  



 
void queue_start (queue *que);  // inicializar cola

int queue_length(queue *que); // tamaño de cola 

int enqueue (queue *que, mthread *thread);  // añade a la cola 

mthread * dequeue (queue *que); // quita de la cola 

int queue_remove(queue *que, mthread *thread); // remueve un nodo 

mthread * queue_get(queue *que, int num); // obtiene elemento de cola 

 
void mthread_init(long period);

 
#endif /* __MTHREAD_H */
