 #include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rrscheduler.h"
#include <sys/time.h>
#include <malloc.h>
#include <signal.h>

 
ucontext_t main; // contexto principal de ejecución 

int cancel_t = 0; // bandera para cancelar el thread 
int total_t = 0; // cantidad total de treads 

queue ready;
queue finish;  // colas para los threads en ready y finish 

thr_t main_t;
thr_t *current;

 
long period_t;

struct itimerval timer; //  tiempo de intervalo para cambiar el hilo a procesar
void start_time() {
	setitimer(ITIMER_VIRTUAL,&timer,0);
}

void stop_time() {
	setitimer(ITIMER_VIRTUAL, 0, 0);
}

 
void queue_start (queue *que) {
        que->front = que->rear = NULL;
	que->count = 0;
}

 
int enqueue (queue *que, thr_t *thread) {
        node *temp = (node *) malloc(sizeof(node));
        if (temp == NULL) {
		return 0;
	}
	temp->data = thread;
	temp->next = NULL;

        if (que->rear) {
                que->rear->next = temp;
                que->rear = temp;
        }
        else
        {
                que->rear = temp;
                que->front = temp;
        }
	++(que->count);

        return 1;
}

thr_t * dequeue (queue *que) {
        if (que->front == NULL) return NULL;
        node *temp = (node *) malloc(sizeof(node));
        temp = que->front;
        que->front = que->front->next;
        thr_t *ret = NULL;
	ret = temp->data;
        free(temp);
	--(que->count);
        return ret;
}

// tamaño de cola
int queue_length (queue *que) {
        return que->count;
}

 
int queue_remove (queue *que, thr_t *thread) {
        node *curr = que->front;
        node *prev = NULL;
        if (curr == NULL) return 0;

  if (que->count <= 0) {
    return 0;
  }

        while (curr) {
                if (curr->data->id == thread->id) {
                        if (prev == NULL) {
                                que->front = curr->next;
                        } else {
                                prev->next = curr->next;
                        }

                        if (curr == que->rear) {
                                que->rear = prev;
                        }
                        free(curr);
			--(que->count);
                        return 1;
                }
        prev = curr;
        curr = curr->next;
        }

        return 0;
}

 

// retorna un hilo basado en la ocurrencia 
thr_t *queue_gete (queue *que, int num) {
        node *temp = que->front;
        if (que->count <= 0) {
                return 0   ;
        }
	int i=0;
	thr_t *data = 0   ;
        while (temp) {
                if (i == num) {
                        data = temp->data;
			return data;
                }
                temp = temp->next;
		++i;
        }

        return 0   ;
}

 
void scheduler () {

        thr_t *prev = NULL;
        thr_t *next = NULL;

        stop_time();

	if ( getcontext(&main) == -1 ) {
		printf("Error al obtener contexto\n");
		exit(EXIT_FAILURE);
	}
        prev = current;

        if (!cancel_t) { // encola el hilo actual
                enqueue(&ready, prev);
        } else {
                cancel_t = 0;
        }

        next = dequeue(&ready); // saca el próximo hilo a procesar 
        if (next == NULL) {
		printf("El hilo no está en la cola\n");
		exit(EXIT_SUCCESS);
	} 
        current = next;
        start_time();
        if ( swapcontext(&(prev->uc), &(next->uc)) == -1 ) {
		printf("Error al intercambiar contexto\n");  
	}
}

 

  