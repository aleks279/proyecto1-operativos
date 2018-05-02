#ifndef __RRSCHEDULER_H
#define __RRSCHEDULER_H
#include <ucontext.h>
 
typedef struct {
	int id;         // identificador del hilo 
	ucontext_t uc; // variable para contexto
	void *retval; // valore de retorno del hilo 
} thr_t;

typedef struct node {
	struct node *next;
	thr_t *data;
} node; 
typedef struct queue {
	struct node *front;
	struct node *rear;
	int count;
} queue; 

 
void queue_start (queue *que); // inicializa la cola 

int queue_length(queue *que); // tamaño de la cola 

int enqueue (queue *que, thr_t *thread); // añade a la cola 

thr_t * dequeue (queue *que); // remueve de cola 

int queue_remove (queue *que, thr_t *thread); // quita elemento de la cola 

thr_t * queue_gete(queue *que, int num); // extrae elemento de la cola 
 
 

 
#endif /* __RRSCHEDULER_H */
