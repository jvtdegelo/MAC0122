/* 
  NAO MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO.

  NO ENTANTO, VOCE PODE ACRESCENTAR ALGO QUE ACHAR CONVENIENTE
  COMO PROTOTIPOS DE NOVAS FUNCOES.

  Interface para funções de uso geral no modulo utils.c 

*/

#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h> /* size_t */
#include <stdio.h>  /* fprintf() */

/*-------------------------------------------------------------*/ 
/* macros de uso geral                                         */
#define ERRO(msg)  fprintf(stderr,"ERRO: %s\n", #msg)
#define AVISO(msg) fprintf(stdout,"AVISO: %s\n", #msg)

/* tamanho maximo dos strings como nome de arquivos e de filmes */

#define TAM_STR 256

#define ENTER '\n'

enum bool 
{
    FALSE, /* = 0, */
    TRUE   /* = 1  */
};
typedef enum bool Bool;

typedef char * String;

/* para busca de padroes */
typedef unsigned char uchar;

/*---------------------------------------------------------------*/
/* prototipos de funcoes                                         */

Bool  achePalavra(unsigned char *pal,   int tPal, 
		  unsigned char *texto, int tTex);

int   strCmp(const char *s1, const char *s2);
 
int   leiaString(char s[], int size);

void *mallocSafe(size_t n);

#endif
