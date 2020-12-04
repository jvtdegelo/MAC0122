/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Joao Victor Texeira Degelo
  NUSP: 11803479

  IMDB: filmes.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/


/*----------------------------------------------------------*/
/* filmes.h e a interface para as funcoes neste modulo      */
#include "filmes.h" 

/*----------------------------------------------------------*/
#include <stdlib.h>  /* NULL, free() */
#include <stdio.h>   /* printf(), scanf() */ 
#include <string.h>  /* strlen(), strncpy(), strcmp(), strtok() */

#include "util.h"    /* Bool, mallocSafe() */
#include "iofilmes.h"
#include "st.h"      /* freeST(), initST(), putFilmeST(), getFilmeST(),
                        showST(), freeST() */

/*----------------------------------------------------------------------
 *  crieFilme
 *
 *  Recebe informacoes dobre um filme 
 *
 *      - DIST  : distribuicao de notas
 *      - VOTOS : numero de votos
 *      - NOTA  : nota do filme 
 *      - ANO   : ano de producao do filme
 *
 *  e cria uma celula do tipo Filme para armazenar essa informacao. 
 *  A funcao retorna o endereco da celula criada.
 */
Filme *
crieFilme (char dist[], int votos, float nota, char *nome, int ano)
{
    Filme *flm;
    int    len = strlen(nome) + 1; /* +1 para o '\0' */
    
    flm = mallocSafe(sizeof *flm);
    
    strncpy(flm->dist, dist, TAM_DIST+1); /* +1 para o '\0' */
    
    flm->votos = votos;
    flm->nota  = nota;
    
    flm->nome = mallocSafe(len*sizeof(char));
    strncpy(flm->nome, nome, len);
    
    flm->ano  = ano;

    flm->prox = flm->ant = NULL; /* paranoia */
    
    return flm;
}

/*----------------------------------------------------------------------
 *  crieListaFilmes
 * 
 *  Cria uma estrutura que representa lista de filmes vazia.
 *  A funcao retorna o endereco dessa estrutura.
 *
 *  Um lista de filmes e representada por uma lista encadeada 
 *  duplamente ligada com cabeca. 
 */
ListaFilmes *
crieListaFilmes()
{
    Filme *cabeca;
    ListaFilmes *lista;
    lista = mallocSafe(sizeof(ListaFilmes));
    char st[TAM_DIST + 1] = "";
    
    cabeca = crieFilme(st, 0, (float)0.0, st, 0);
    lista -> cab = cabeca;
    lista -> nFilmes = 0;
    cabeca -> prox = cabeca;
    cabeca -> ant = cabeca;
    return lista;
}

/*----------------------------------------------------------------------
 *  libereListaFilmes(lst)
 *
 *  Recebe um ponteiro lst para uma estrutura que representa uma lista 
 *  de filmes e libera toda a memoria alocada para a lista.
 *
 *  Esta funcao utiliza a funcao libereFilme().
 */

void
libereListaFilmes(ListaFilmes *lst)
{
    Filme *cabeca = lst->cab;
    Filme *atual = cabeca -> prox;
    Filme *prox;
    
    while(atual != cabeca){
        prox = atual->prox;
        libereFilme(atual);
        atual = prox;
    }
    cabeca->prox = cabeca;
    cabeca->ant = cabeca; 
    lst->nFilmes = 0;
    return;
}

/*----------------------------------------------------------------------
 *  libereFilme
 *
 *  Recebe um ponteiro FLM para uma estrutura que representa um 
 *  filme e libera a area alocada.
 *
 */
void 
libereFilme(Filme *flm)
{
    if(flm->nome != NULL) free(flm->nome);
    free(flm);
}

/*----------------------------------------------------------------------
 *  insiraFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa um filme.
 *
 *  A funcao insere o filme na lista.
 *  
 */
void 
insiraFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *cabeca, *proximo;

    cabeca = lst -> cab;
    proximo = cabeca -> prox;
    flm -> prox = proximo;
    flm -> ant = cabeca;
    cabeca -> prox = flm;
    proximo -> ant = flm;
    lst->nFilmes +=1;
}


/*---------------------------------------------------------------------
 *  contemFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao retorna  TRUE se o filme esta na lista e 
 *  FALSE em caso contrario. 
 *
 *  Consideramos que dois filmes f e g sao iguais se
 *
 *       - f->nome e' igual a g->nome 
 *       - f->nota e' igual a g->nota
 *       - f->ano  e' igual a g->ano 
 *
 *  Para comparar dois nomes voce pode usar alguma funcao da 
 *  bibliteca do c  como strcmp, strncmp (string,h) 
 *  ou a funcao strCmp (util.h).
 *
 */
Bool 
contemFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *cabeca = lst->cab;
    Filme *f;

    for(f = cabeca -> prox; f != cabeca; f = f-> prox){
        if(f -> ano == flm -> ano && f-> nota == flm -> nota && strCmp(f -> nome, flm -> nome) == 0){
            return TRUE;
        }
    }
    return FALSE;
}

/*----------------------------------------------------------------------
 *  removaFilme
 *
 *  Remove da lista de filmes LST o filme apontado por FLM.
 *    
 *  Pre-condicao: a funcao supoe que o filme FLM esta 
 *                na lista LST.
 */
void 
removaFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *prox, *ant;

    prox = flm->prox;
    ant = flm->ant;

    prox->ant = ant;
    ant->prox = prox;

    libereFilme(flm);
    (lst->nFilmes)--;
    return;
}

/*----------------------------------------------------------------------
 *  mergeSortFilmes
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo mergeSort recursivo adaptado para listas encadeadas
 *  duplamente ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 *
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
mergeSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    int n = lst->nFilmes, i;
    Filme *f = lst -> cab, *f_prox;
    Filme *atual, *f1, *f2;
    ListaFilmes *lst1 = crieListaFilmes();
    ListaFilmes *lst2 = crieListaFilmes(); 
    
    if (n == 1) return;
    for (i = 0; i < n/2; i++)   f = f -> prox;
    f_prox = f -> prox;
    lst1 -> nFilmes = n/2;
    lst2 -> nFilmes = n - n/2;

    // criando a lista de filmes da primeira metade
    (lst1->cab)-> prox = (lst -> cab) -> prox;
    ((lst->cab) -> prox) -> ant = lst1-> cab;
    (lst1->cab) -> ant = f;
    f -> prox = lst1->cab;

    //criando a lista de filmes da segunda metade
    (lst2->cab)-> prox = f_prox;
    f_prox -> ant = lst2->cab;
    (lst2->cab) -> ant = (lst -> cab) -> ant;
    ((lst->cab) -> ant) -> prox = lst2-> cab;
    
    mergeSortFilmes(lst1, criterio);
    mergeSortFilmes(lst2, criterio);    

    atual = lst -> cab;
    f1 = (lst1->cab)->prox;
    f2 = (lst2->cab)->prox;

    while (f1 != lst1->cab || f2 != lst2->cab){
        if(f1 == lst1->cab){
            atual -> prox = f2;
            f2->ant = atual;
            atual = f2;
            f2 = f2->prox;
        }
        else if(f2 == lst2->cab){
            atual -> prox = f1;
            f1->ant = atual;
            atual = f1;
            f1 = f1->prox;
        }
        else if((criterio == NOTA && f1->nota <= f2->nota) ||(criterio == NOME && strCmp((const char *)(f1->nome), (const char *)(f2->nome))> 0)){
            atual -> prox = f2;
            f2->ant = atual;
            atual = f2;
            f2 = f2->prox;
        }
        else{
            atual -> prox = f1;
            f1->ant = atual;
            atual = f1;
            f1 = f1->prox;
        }
    }
    atual->prox = lst->cab;
    (lst->cab)->ant = atual;
    libereFilme(lst1->cab);
    libereFilme(lst2->cab);
    free(lst1);
    free(lst2);
    return;
}

/*----------------------------------------------------------------------
 *  quickSortFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo quickSort adaptado para listas encadeadas duplamente
 *  ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 * 
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
quickSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    if(lst->nFilmes <=1) return;

    Filme *f = lst->cab->prox, *f_prox;
    Filme *q = lst->cab->ant;
    ListaFilmes *lst1 = crieListaFilmes();
    ListaFilmes *lst2 = crieListaFilmes();

    while(f != q){
        f_prox = f->prox;
        if((criterio == NOTA && f->nota>=q->nota) || (criterio == NOME && strcmp(f->nome, q->nome)<0)){
            lst1->cab->prox->ant = f;
            f->prox = lst1->cab->prox;
            lst1->cab->prox = f;
            f->ant = lst1->cab;
            lst1->nFilmes = lst1->nFilmes + 1;
        }
        else {
            lst2->cab->prox->ant = f;
            f->prox = lst2->cab->prox;
            lst2->cab->prox = f;
            f->ant = lst2->cab;
            lst2->nFilmes = lst2->nFilmes + 1;
        }
        f = f_prox;
    }
   
    quickSortFilmes(lst1, criterio);
    quickSortFilmes(lst2, criterio);
    if(lst1->nFilmes == 0){
        lst->cab->prox = q;
        q->ant = lst->cab;
        q->prox = lst2->cab->prox;
        lst2->cab->prox->ant = q;
        lst2->cab->ant->prox = lst->cab;
        lst->cab->ant = lst2->cab->ant;
        libereFilme(lst1->cab);
        libereFilme(lst2->cab);
        free(lst1);
        free(lst2);
        return;
    }
    if(lst2->nFilmes == 0){
        lst->cab->ant = q;
        q->prox = lst->cab;
        q->ant = lst1->cab->ant;
        lst1->cab->ant->prox = q;
        lst1->cab->prox->ant = lst->cab;
        lst->cab->prox = lst1->cab->prox;
        libereFilme(lst1->cab);
        libereFilme(lst2->cab);
        free(lst1);
        free(lst2);
        return;
    }
    lst->cab->prox = lst1->cab->prox;
    lst1->cab->prox->ant = lst->cab;

    lst1->cab->ant->prox = q;
    q->ant = lst1->cab->ant;

    lst2->cab->prox->ant = q;
    q->prox = lst2->cab->prox;

    lst2->cab->ant->prox = lst->cab;
    lst->cab->ant = lst2->cab->ant;
    
    libereFilme(lst1->cab);
    libereFilme(lst2->cab);
    free(lst1);
    free(lst2);
    return;
}

/*----------------------------------------------------------------------
 *  hashFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e distribui as palavras que
 *  ocorrem nos nomes do filmes em uma tabela de dispersao 
 *  (hash table):
 *
 *     http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 *
 *  Antes de inserir uma palavra na tabela de dispersao todas a
 *  letras da palavra devem ser convertidas para minusculo. Com
 *  isto faremos que a busca de filmes que possuam uma dada
 *  palavra em seu nome nao seja 'case insensitive'. Para essa
 *  tarefa a funcao tolower() pode ser utilizada.
 *
 *  Esta funcao utiliza as funcoes freeST(), initST() e putFilmeST()
 *
 *  Para obter as palavras podemos escrever uma funcao peguePalavra()
 *  inspirada na funcao pegueNome do modulo lexer.c do EP3/EP4 ou
 *  ainda utilizar a funcao strtok() da biblioteca string.h:
 *  
 *      http://linux.die.net/man/3/strtok    (man page)
 *
 */
void
hashFilmes(ListaFilmes *lst)
{
    AVISO(hashFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}
