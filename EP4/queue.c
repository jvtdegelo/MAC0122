/*
 *  distancias.c
 */
#include <stdio.h>
#include <stdlib.h>

#include "item.h"

static void *mallocSafe (unsigned int n);

/*
 * FILA: uma implementacao com lista encadeada sem cabeca
 */

typedef struct queueNode* Link;
struct queueNode { 
  Item conteudo; 
  Link prox; 
};

struct queue { /* aqui esta especificado o que e' */
  Link inicio; /* uma fila: dois apontadores para struct queueNode */
  Link fim;    
}; 

typedef struct queue *Queue;

static Link new(Item conteudo, Link prox) { 
  Link x = (Link) mallocSafe(sizeof *x);
  x->conteudo = conteudo; x->prox = prox;     
  return x;                         
}     
                              
Queue queueInit(int maxN) { 
  Queue q = (Queue) mallocSafe(sizeof *q); 
  q->inicio = NULL; 
  q->fim = NULL; 
  return q;
}

int queueEmpty(Queue q) { 
  return q->inicio == NULL; 
}

void queuePut(Queue q, Item conteudo) { 
  if (q->inicio == NULL) { 
    q->fim = new(conteudo, NULL);
    q->inicio = q->fim; 
    return; 
  }
  q->fim->prox = new(conteudo, NULL); 
  q->fim = q->fim->prox;
}

Item queueGet(Queue q) { 
  Item conteudo = q->inicio->conteudo;
  Link t = q->inicio->prox;
  free(q->inicio); 
  q->inicio = t;
  return conteudo;
}

void queueFree(Queue q) { 
  while (q->inicio != NULL) { 
    Link t = q->inicio->prox;
    free(q->inicio);
    q->inicio = t;
  }
  free(q);
}

static void *mallocSafe (unsigned int n) {
  void *p;
  
  p = malloc(n);
  if (p == NULL) {
    fprintf(stderr,"queue: malloc de %u bytes falhou.\n", n);
    exit (-1);
  }
  return p;
}