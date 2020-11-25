/*
 * MAC0122 Princípios de Desenvolvimento de Algoritmos
 *
 * Interface para uma Tabela de Simbolos implementada atraves
 * de uma tabela de dispersão (hash table). A
 *
 * As chaves (keys) para a tabelas de símbolos sao palavras
 * que aparecem no tituto dos filmes. Os objetos (itens) da
 * tabela sao os filmes que possuem a palavra em seus titulos.
 *
 */

/*
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
 *
 * Aqui estao declarados o tipo CelST e o prototipo das funcoes 
 * que manipulam a tabela de simbolos.
 *  
 */

#ifndef _ST_H
#define _ST_H

#include "filmes.h"  /* Filme  */
#include "util.h"    /* String,  */

/*----------------------------------------------------------*/
/* estrutura para a lista de ponteiros para filmes          */
typedef struct ptrFilmes ListaPtrFilmes;
struct ptrFilmes
{
    /* ponteiro para um filme na lista de filmes */
    Filme *ptrFlm;

    /* ponteiro para o proximo ponteir de filme na lista */
    ListaPtrFilmes *proxPtr;  
};

/*-----------------------------------------------------------*/
/* prototipos das funcoes que manipulam a tabela de simbolos */
void  
initST();

ListaPtrFilmes *
getFilmeST(String palavra);

void  
putFilmeST(String palavra, Filme *flm);

void  
showST(); 

void  
freeST();

#endif /* _ST_H */
