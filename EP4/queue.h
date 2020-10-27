/*
 * queue.h 
 * INTERFACE: funcoes para manipular a fila 
 * ATENCAO: Esta interface permite que varias filas sejam 
 *          utilizadas simultaneamente.
 */
typedef struct queue *Queue;

Queue queueInit(int);
int queueEmpty(Queue);
void queuePut(Queue,Item);
Item queueGet(Queue);
void queueFree(Queue);

/*void queuedump(Queue); */