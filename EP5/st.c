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

  st.c
  IMDB 2020

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
 */

/* 
 * Implementacao de uma tabela de simbolos atraves de uma tabela
 * de dispersao ('hash table').
 * 
 * As colisoes devem ser resolvidas através de listas encadeadas 
 * ('separate chaining').
 * 
 * O código a seguir é uma adaptacao do codigo de D.E. Knuth
 * usado no SGB para a estrutura de dados do Sedgewick que esta
 * nas notas de aula de Paulo Feofiloff:
 * 
 *    http://www.ime.usp.br/~pf/algoritmos_para_grafos/
 * 
 * Veja tambem
 * 
 *    http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *
 * OBSERVACAO: A busca na tabela de dispersao e' 'case insensitive'
 */

/* interface para o uso da funcao deste módulo */
#include "st.h"      /* ListaPtrFilmes */

#include <stdio.h>   /* printf() */
#include <stdlib.h>  /* free() */
#include "string.h"  /* strlen(), strcpy(),... */
#include "filmes.h"  /* Filmes */
#include "util.h"    /* Bool, String, strCmp(), mallocSafe() */
#include <ctype.h>   /* tolower() */


/*-----------------------------------------------------------*/
/* Tamanho da Tabela de Dispersao 
 *
 * 65521 e o maior primo menor que 2^16 = 65536 Observacao:
 * para este EP talvez o tamanho da tabela pudesse ser
 * menor, 65521 talvez seja um exagero...  
 *
 * No EP M eh uma constante, mas podia ser uma variavel.
 */ 
#define M 65521


/*-----------------------------------------------------------*/
/* funcao que calcula o codigo de dispersao (hash code) de   
 * cada chave/palavra.
 */
static int
hash(String palavra);

/*----------------------------------------------------------*/
/* Estrutura Basica da Tabela de Simbolos: 
 * 
 * definicao de uma celula/no' da lista de colisoes
 */
typedef struct celST CelST;
struct celST 
{
    /* ponteiro para uma palavra que ocorre no titulo de algum filme */
    String palavra; 

    /* ponteiro para lista de ponteiros para filmes que possuem a
     * palavra em seu titulo */
    ListaPtrFilmes *iniListaPtr;

    /* ponteiro para a proxima celula na tabela de dispersao */
    CelST *proxST;
};

/*-----------------------------------------------------------*/
/* ponteiros para as M listas de colisoes: para cada h em
 * [0..M-1], hashHead[h] e um ponteiro para o inicio da lista
 * encadeada de celulas do tipo CelST que contem as palavras cujo
 * valor da funcao de hash é h.
 */
static CelST *hashHead[M];

/* numero de chaves na tabela de simbolos */
static int nChaves = 0; 


/* 
 * hash
 *
 * RECEBE uma string PALAVRA e RETORNA o seu codigo de dispersao (=
 * 'hash code').  Em outras palavras, a funcao retorna o indice h da
 * tabela de dispersao que aponta para a lista ligada das palavras
 * (chaves) que tem h como codigo. E' nessa lista o lugar de PALAVRA
 * na tabela de simbolos.
 *
 * OBSERVACAO: todas as letras da palavra sao consideradas como
 *     minusculas. Hmmm. Acho que faz sentido a busca ser 'case
 *     insensitive'.
 *     
 */
static int
hash(String palavra)
{
    int i, h = 0;
    int primo = 251;
    for (i = 0; palavra[i] != '\0'; i++)
        h = (h * primo + tolower(palavra[i])) % M;
    return h;
}

/*-----------------------------------------------------------*/
/*
 *  initST
 *
 *  Inicializa a tabela hashHead que apontara para o inicio de
 *  cada uma das M listas de colisoes.
 */
void
initST()
{
    int h;
    for (h = 0; h < M; h ++)
        hashHead[h] = NULL;

    return;
}

/*-----------------------------------------------------------*/
/*
 *  putFilmeST
 * 
 *  RECEBE um string PALAVRA e um ponteiro FLM para um filme
 *  que possui a palavra em seu nome. A funcao procura a 
 *  PALAVRA na tabela de dispersao (tabela de simbola) e 
 *  
 *     - se PALAVRA nao e' encontrada:
 *       
 *       Uma nova celula CelST e criada e inserida na 
 *       lista de colisoes. 
 *       O string PALAVRA deve ser copiada para o campo palavra
 *       da nova celula.
 *       O ponteito FLM deve ser inserido na lista dos ponteiros
 *       para filmes dessa nova celula (iniListaPtr).
 *
 *     - se PALAVRA e' encontra:
 *
 *       O ponteito FLM deve ser inserido na lista dos ponteiros
 *       para filmes da celula (iniListaPtr) que contem PALAVRA.
 *        
 * Para fazer a busca de PALAVRA na tabela de dispersao esta 
 * funcao deve usar a funcao hash().
 * 
 * Para comparar strings utilize a funcao strCmp() (util.h).
 * Para strings podem ser utilizadas funcoes da biblioteca
 * string.h:  strcpy(), strCmp()...
 * 
 * OBSERVACAO: A busca por PALAVRA e' 'case insensitive'.
 */
void
putFilmeST(String palavra, Filme *flm)
{
    int h = hash(palavra);
    CelST *atual = hashHead[h];
    if(atual == NULL) nChaves++;
    
    while(atual != NULL){
        if(strcmp(atual->palavra, palavra) == 0){
            break;
        }
        atual = atual-> proxST;
    }
    
    if(atual == NULL){
        //a celST que contem a palavra esta armazenada no atual
        CelST *new_cel;
        ListaPtrFilmes *lista;
        String pal;

        pal = mallocSafe((strlen(palavra)+1)*sizeof(char));
        strcpy(pal, palavra);
        lista = mallocSafe(sizeof(ListaPtrFilmes));
        new_cel = mallocSafe(sizeof(CelST));
        lista->ptrFlm = flm;
        lista->proxPtr = NULL;
        new_cel->iniListaPtr = lista;
        new_cel->proxST = hashHead[h];
        hashHead[h] = new_cel;
        new_cel->palavra = pal;
    }
    else{
        ListaPtrFilmes *lista;
        
        lista = mallocSafe(sizeof(ListaPtrFilmes));
        lista->ptrFlm = flm;
        lista->proxPtr = atual->iniListaPtr;
        atual->iniListaPtr = lista;
    }
}

/*-----------------------------------------------------------*/
/*
 *  getFilmeST
 *
 *  RECEBE e procura um string PALAVRA na tabela de dispersao
 *  (tabela de simbola) e:
 *
 *     - se PALAVRA nao é encontrada a funcao RETORNA NULL.
 *
 *     - se PALAVRA é encontrada a funcao RETORNA o ponteiro 
 *       para o inicio da lista de ponteiros de filmes que 
 *       contem a palavra (campo iniListaPtr da celST que 
 *       contem PALAVRA).
 *        
 * Para fazer a busca de PALAVRA na tabela de dispersão esta 
 * funcao deve usar a funcao hash().
 * 
 * Para comparar strings utilize a funcao strCmp() (util.h).
 *
 * OBSERVACAO: A busca por PALAVRA é case insensitive.
 */
ListaPtrFilmes *
getFilmeST(String palavra)
{
    int h = hash(palavra);
    CelST *atual = hashHead[h];

    while(atual != NULL){
        if(strcmp(atual->palavra, palavra) == 0){
            return atual->iniListaPtr;
        }
        atual = atual-> proxST;
    }
    return NULL;
}

/*-----------------------------------------------------------*/
/*
 *  showST
 *
 *  Mostra todas as palavras na tabela de simbolos.
 *
 *  Exemplo de tabela produzida por esta funcao para o arquivo 
 *  10-filmes.list:
 *
 *  . . . . . . . . . . . . . . . . . . . . . . .
 *  Tabela de simbolos: { codigo: lista de chaves }
 *  {   161: 'en' }
 *  {  1437: 'Tore' }
 *  {  1782: 'der' }
 *  {  2627: 'Paix�o' }
 *  {  3491: 'dem' }
 *  {  6158: 'a' }
 *  {  6268: 'Alyonka' }
 *  {  7486: 'Always' }
 *  { 11226: 'Batman' }
 *  { 12868: 'Al�m' }
 *  { 14213: 'vor' }
 *  { 15659: 'Robin' }
 *  { 18881: 'for' }
 *  { 19199: 'Heart' }
 *  { 20134: 'Am' }
 *  { 24176: 'Pleasure' }
 *  { 28125: 'Bridesmaid' }
 *  { 28682: 'Alyas' }
 *  { 33039: 'parusa' }
 *  { 36475: 'Al�' }
 *  { 38348: 'Da' }
 *  { 42245: 'Anfang' }
 *  { 48446: 'My' }
 *  { 49084: 'Alye' }
 *  { 51770: 'Brunnen' }
 *  { 54432: 'Seitensprung' }
 *  { 60922: 'in' }
 *  { 61261: 'war' }
 *   
 */
void
showST()
{
    CelST *atual, *h;
    int i;
    printf("Tabela de simbolos: { codigo: lista de chaves }\n");
    for(i = 0; i < M; i++) {
        atual = hashHead[i];
        if(atual == NULL) continue; 
        printf("{ %5d: ", i);
        for(h = atual; h != NULL; h = h->proxST){
            printf("'%s' ", h->palavra);
        }
        printf("}\n");
    }
}

/*-----------------------------------------------------------*/
/*
 *  freeST
 *
 *  LIBERA toda a memoria alocada para a tabela de dispersao
 *  (tabela de simbolos).
 */
void
freeST()
{
    CelST *atual, *h, *h_prox;
    int i;
    for(i = 0; i < M; i++) {
        atual = hashHead[i];
        for(h = atual; h != NULL; h = h_prox){
            ListaPtrFilmes *lista = h->iniListaPtr, *prox_lista;
            h_prox = h->proxST;
            if(h->palavra != NULL) free(h->palavra);
            while(lista != NULL){
                prox_lista = lista->proxPtr;
                free(lista);
                lista = prox_lista;
            }
            free(h);
        }
    }
}

