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

  IMDB: util.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
  - função achaPalavra baseada nos algoritmos desenvolvidos durante a aula, disponível em:
  <https://www.ime.usp.br/~cris/aulas/20_2_122/slides/aula23.pdf>
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* 
 * IMPLEMENTACAO de funcoes de uso geral 
 *
 */

/* interface para este modulo */
#include "util.h" 

#include <stdio.h>     /* fgets(), printf() */
#include <stdlib.h>    /* malloc(), NULL, EXIT_FAILURE */
#include <string.h>    /* strlen() */
#include <ctype.h>     /* tolower() */

/* tamanho maximo de um string neste programa como nome de arquivo 
   ou nome um filme */
#define TAM_STR 256

#define DEBUG


/*----------------------------------------------------------------------
 * achePalavra
 *
 * Recebe o string PAL com tamanho TPAL, e outro string TEXTO com
 * tamanho TTEX e retornar TRUE caso a PAL ocorra em TEXTO, e
 * FALSE em caso contrário. 
 * 
 * A busca nao deve levar em consideracao se as letras em PAL ou
 * TEXTO sao em caixa alta (maiusculas) ou caixa normal
 * (minusculas). Assim, a busca deve ser 'case insensitive'.
 *
 */
int *
preProcessamento(unsigned char p[], int m) {
    int i, r, j, *alcance;
    alcance = malloc((m+1)*sizeof(int));
    for (i = m; i >= 1; i--) {
        j = m-1; r = 0;
        while (m-r >= i && j-r >= 1){
            if (tolower(p[m-r]) == tolower(p[j-r])) r += 1;
            else j -= 1, r = 0;
        }
        alcance[i] = j;
    }
    return alcance;
}
Bool
achePalavra(unsigned char *pal, int tPal, unsigned char *texto, int tTex)
{
    int *alcance;
    int ult[256];
    int i, r, k, ocorrs;
    // mudamos as strings de [0...n-1] [para 1...n]
    int m = tPal, n = tTex;
    unsigned char *p, *t;
    p = mallocSafe((m+1)*sizeof(char));
    t = mallocSafe((n+1)*sizeof(char));
    for(i = 1; i <= m; i++) p[i] = tolower(pal[i-1]);
    for(i=1; i <= n; i++) t[i] = tolower(texto[i-1]);
    
    /* pre-processamento da palavra p */
    alcance = preProcessamento(p, m);
    for (i=0; i < 256; i++) ult[i] = 0;
    for (i=1; i <= m; i++) ult[p[i]] = i;

    /* busca da palavra p no texto t */
    ocorrs = 0; k = m;
    while (k <= n) {
        r = 0;
        while (r < m && p[m-r] == t[k-r])
            r += 1;
        if (r == m) ocorrs += 1;
        if (k == n) k += 1;
        else if (r == 0) k += 1;
        else{
            if(alcance[m-r+1]>ult[t[k+1]] + 1) k += m - ult[t[k+1]] + 1;
            else k += m - alcance[m-r+1];
        }
    }
    free(alcance);
    free(p);
    free(t);
    if(ocorrs == 0) return FALSE;
    else return TRUE;
}


/*----------------------------------------------------------------------
 *   strCmp(const char *s1, const char *s2)
 *
 *   Comportamento identico ao da funcao strcmp da interface string.h
 *   da libC. A unica diferenca e que esta funcao desconsidera se as
 *   letras sao maiusculas ou mininusculas.
 *
 *   copiado da  gLibc
 *
 *   Ver http://fossies.org/dox/glibc-2.20/string_2strcmp_8c_source.html
 *   ou a copia nos slides das aulas de MAC0122.
 *     
 *   ------------------------------------------------------------------
 *   NAME
 *      strcmp - compare two strings
 *
 *   SYNOPSIS
 *      #include <string.h>
 *
 *      int strcmp(const char *s1, const char *s2);
 *
 *   DESCRIPTION
 *      The  strcmp() function compares the two strings s1 and s2.  
 *      It returns an integer less than, equal to, or greater than
 *      zero if s1 is found, respectively, to be less than, to match, 
 *      or be greater than s2.
 *
 */

int  
strCmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    unsigned char c1, c2;
 
    do
    {
        c1 = tolower((unsigned char) *p1++); /* tolower foi acrescentado */ 
        c2 = tolower((unsigned char) *p2++); /* tolower foi acrescentado */
        if (c1 == '\0')
            return c1 - c2;
    }
    while (c1 == c2);
 
    return c1 - c2;
}

/* 
 * Sobre a GLIBC:
 *
 * The GNU C Library, commonly known as glibc, is the GNU Project's 
 * implementation of the C standard library. Despite its name, it now 
 * also directly supports C++ (and, indirectly, other programming languages). 
 * It was started in the early 1990s by the Free Software Foundation (FSF) 
 * for their GNU operating system.
 * 
 * Released under the GNU Lesser General Public License,[3] glibc is free software. 
 * The GNU C Library project provides the core libraries for the GNU system and 
 * GNU/Linux systems, as well as many other systems that use Linux as the kernel. 
 * These libraries provide critical APIs including ISO C11, POSIX.1-2008, BSD, 
 * OS-specific APIs and more. These APIs include such foundational facilities 
 * as open, read, write, malloc, printf, getaddrinfo, dlopen, pthread_create, 
 * crypt, login, exit and more.
 */


/*--------------------------------------------------------------
 *   leiaString
 *
 *   RECEBE como parametros um string STR e um inteiro SIZE e LE
 *   da entrada padrao no maximo SIZE-1 caracteres e ARMAZENA em
 *   STR. A leitura para depois de um ENTER e o ENTER _nao_ e
 *   incluido no string STR.  A funcao retorna o numero de
 *   caracteres no string.
 *
 *   Pre-condicao: SIZE <= TAM_STR
 *
 *   . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 *   > man fgets
 *   char *fgets(char *s, int size, FILE *stream);
 *
 *   fgets() reads in at most one less than size characters from
 *   stream and stores them into the buffer pointed to by s.
 *   Reading stops after an EOF or a newline.  If a newline is
 *   read, it is stored into the buffer.  A terminating null byte
 *   ('\0') is stored after the last character in the buffer.
 */

int 
leiaString(char str[], int size)
{
    int len;
    char s[TAM_STR];

    /* remove qualquer newline que estava no buffer */
    scanf(" ");

    /* leitura do string: ler info sobre fgets() */
    fgets(s, TAM_STR, stdin);

    /* sobreescreve  um possivel newline do final com '\0' */
    len = strlen(s); 
    if (s[len-1] == ENTER) 
    {
        len--;
        s[len] = '\0';
    }

    if (len > size)
    {
        s[size-1] = '\0';
        len = size-1;
    }
    strncpy(str, s, size);

#ifdef DEBUG
    printf("AVISO: leiaString: string lido '%s' tem %d caracteres\n", s, len);
#endif
    return len;
}

/*---------------------------------------------------
 *   mallocSafe() 
 *
 *   http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
 */
void * 
mallocSafe(size_t n)
{
    void * p;

    p = malloc(n);
    if (p == NULL) 
    {
        printf("ERRO na alocacao de memoria.\n\n");
        exit(EXIT_FAILURE);
    }

    return p;
}
