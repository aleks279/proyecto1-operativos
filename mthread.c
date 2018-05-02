#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mthread.h"
#include <sys/time.h>
#include <malloc.h>
#include <signal.h>

// contexto principal a ejecutar 
ucontext_t maincontext;

int cancel_current = 0;  // indica el hilo actual 
int total_thr_count = 0; // indica el total de hilos 

queue ready_q; // cola para los hilos ready
finish_q; // cola para los hilos finish

 
mthread  *current;

 
long ṕeriod; // tiempo de intervalo en milisegundos (quantum)

struct itimerval timer;
void time_init() {
	setitimer(ITIMER_VIRTUAL,&timer,0);
}

void time_cancel() {
	setitimer(ITIMER_VIRTUAL, 0, 0);
}

/*Queue functions' definition*/
void queue_start (queue *que) {
        que->front = que->rear = NULL;
	que->count = 0;
}

 // Anade un nodo a la cola
int enqueue (queue *que, mthread  *thread) {
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

// Remueve un nodo de la cola 

mthread  * dequeue (queue *que) {
        if (que->front == NULL) return NULL;
        node *temp = (node *) malloc(sizeof(node));
        temp = que->front;
        que->front = que->front->next;
        mthread  *ret = NULL;
	ret = temp->data;
        free(temp);
	--(que->count);
        return ret;
}

// Tamaño de cola 
int queue_length (queue *que) {
        return que->count;
}

// remueve un hilo de la cola 
int queue_remove (queue *que, mthread  *thread) {
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

// Retorna un hilo basado en su ocurrencia 

mthread  *queue_get (queue *que, int num) {
        node *temp = que->front;
	 
        if (que->count <= 0) {
                return 0   ;
        }
	int i=0;
	mthread  *data = 0   ;
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

 
 

 

// Función principal el scheduler 
void scheduler () {
        mthread  *prev , *next = NULL;
        time_cancel();
	if ( getcontext(&maincontext) == -1 ) {
		printf("error al cambiar de contexto\n");
		exit(EXIT_FAILURE);
	}
        prev = current;

        if (!cancel_current) {
                enqueue(&ready_q, prev);
        } else {
                cancel_current = 0;
        }

        next = dequeue(&ready_q);
        if (next == NULL) {
		printf("El hilo no está en ready\n");
		exit(EXIT_SUCCESS);
	}   // no hay hilo en cola 
        current = next;
        time_init();
        if ( swapcontext(&(prev->uc), &(next->uc)) == -1 ) {
		printf("error al intercambiar contexto\n");  // llama al sig thread 
	}
}

 

 
void mthread_init (long period) {
	queue_start(&ready_q);
	queue_start(&finish_q);

	// intervalos de tiempo en milisegundos 
	timer.it_value.tv_sec=period/1000000;
	timer.it_value.tv_usec=period;
	timer.it_interval.tv_sec=period/1000000;
	timer.it_interval.tv_usec=period;
	time_init();
 
	signal(SIGVTALRM,scheduler); // inicia el signal handler 

	//guarda contexto del thread 
	main_t.id = -1;
	if ( getcontext(&(main_t.uc)) == -1) { 
		printf("error obteniendo el contexto\n");
		exit(EXIT_FAILURE);
	}	
	current = &main_t; // guarda el hilo principal como actual

}

  
