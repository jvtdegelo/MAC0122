/*    
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO-PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome:Joao Victor Texeira Degelo   
  NUSP:11803479

  ep3.c

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

  - A biblioteca stackS.h e item.h sao as bibliotecas que a professora 
  desenvolveu durante a aula, e estao disponiveis em:
  <https://www.ime.usp.br/~cris/mac0122/programas/polonesa/stack4/>,
   alem delas, me baseei na logica utilizada no polonesa.c 
  (que esta no mesmo link) para construir o caso em que em que atribuimos
   a um polinomio uma expressao infixa
  - Foi anexado tambem um Makefile para compilar esse cliente, que necessita 
  dos seguintes arquivos:
  * ep3.c
  * polinomios.c
  * polinomios.h
  * stackS.c
  * stackS.h
  * item.h 
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/
#include<stdio.h>
#include<stdlib.h>
#include "polinomios.h"
#include "item.h"
#include "stackS.h"
#define maxtam 103//tamanho máximo das strings

polinomio var[26];

void descarta_resto_da_linha();
polinomio *calcula_infixa(char *);
void *malloc_safe(size_t);
void faz_operacao(Stack, char);

void *malloc_safe (size_t nbytes){
   void *ptr;

   ptr = malloc (nbytes);
   if (ptr == NULL) {
      printf ("Socorro! malloc devolveu NULL!\n");
      exit (EXIT_FAILURE);
   }
   return ptr;
}

void descarta_resto_da_linha() {
  //funcao que chamamos apos finalizar uma operacao da calculadora
  //para evitar que caracteres "inuteis", interfiram nas proximas operacoes
  char c;
  
  do {
    scanf("%c", &c); 
  } while (c != '\n' && c != EOF);
}

polinomio *calcula_infixa (char *infixa){
  //funcao que recebe a expressao em notacao infixa e 
  //calcula o polinomio equivalente a expressao
  //para tornar mais eficiente o processo, transcrevemos
  //a expressao para a notacao posfixa, e enquanto fazemos isso
  //vamos calculando a expressao

  Stack sope;
  Stack sval;
  char *item;
  polinomio *v, res;
  int i = -1;

  sope = stackInit(maxtam);
  sval = stackInit(maxtam);
  while (infixa [++i] !=  '\0'){
    if (infixa [i] == '(')
      stackPush (sope, &infixa[i]);
  
    else if (infixa [i] == ')'){
      while (*(item = stackPop (sope)) != '(')
        faz_operacao (sval,*item);
    }

    else if (infixa [i] == '+' || infixa [i] == '-'){
      while (!stackEmpty(sope) && *(item = stackTop(sope)) != '('){
        item = stackPop(sope);
        faz_operacao(sval, *item);
      }
      stackPush(sope, &infixa[i]);
    }

    else if(infixa[i] == '*' || infixa[i] == '/' || infixa[i] == '%'){
      while (!stackEmpty(sope) && *(item = stackTop(sope)) != '(' 
	                       && *item != '+' && *item != '-') {
	      item = stackPop(sope);
	      faz_operacao(sval, *item);
      }
      stackPush(sope, &infixa[i]);
    }
    
    else if(infixa[i] == '~'){
      while (!stackEmpty(sope) && *(item = stackTop(sope)) == '~') {
	      item = stackPop(sope);
	      faz_operacao(sval, *item);
      }
      stackPush(sope, &infixa[i]);
    }
    else{
      v = malloc_safe(sizeof(polinomio));
      *v = copia(var[infixa[i] - 'a']);
      stackPush(sval, v);
    }
  }
  while(!stackEmpty(sope)){
    item = stackPop(sope);
    faz_operacao(sval, *item);
  }
  v = stackPop(sval);
  res = *v;
  free(v);
  stackFree(sval); stackFree(sope);
  return res;
}

void faz_operacao(Stack sval, char op){
  //funcao que recebe a stack de polinomios e uma operacao
  //e com isso calcula a a operacao com os dois primeiros 
  //polinomios da stack
  polinomio *v, *v1, *v2;

  v = malloc_safe(sizeof(polinomio));
  if(op == '+'){
    v1 = stackPop(sval);
    v2 = stackPop(sval);
    *v = soma(*v1, *v2);
  }
  else if(op == '-'){
    v1 = stackPop(sval);
    v2 = stackPop(sval);
    *v = subt(*v2, *v1);
  }
  else if(op == '*'){
    v1 = stackPop(sval);
    v2 = stackPop(sval);
    *v = mult(*v1, *v2);
  }
  else if(op == '%'){
    v1 = stackPop(sval);
    v2 = stackPop(sval);
    *v = rest(*v2, *v1);
  }
  else if(op == '/'){
    v1 = stackPop(sval);
    v2 = stackPop(sval);
    *v = quoc(*v2, *v1);
  }
  else if(op == '~'){
    v1 = stackPop(sval);
    *v = nega(*v1);
  }
  stackPush(sval, v);
  libera(*v1); free(v1);
  if(op != '~'){
    libera(*v2);
    free(v2);
  }
}

int main(){
    int i;

    for(i=0; i<26; i++) var[i] = cria();    
    printf("*************************\nCalculadora de polinomios\n*************************\n\n");
    while(1){
      char p, op;

      printf("Digite uma expressão ou quit para sair do programa:\n> ");
      scanf("%c%c", &p, &op);
      
      if(op == ':'){
        //devemos substituir o polinomio atual pelo lido
        libera(var[p-'a']);
        var[p-'a'] = leia();
        impr(p, var[p-'a']);
        descarta_resto_da_linha();
      }
      else if(op == '?'){
        //devemos imprimir o polinomio
        impr(p, var[p-'a']);
        descarta_resto_da_linha();
      }
      else if(op == '='){
        //recebe expressao infixa
        //devemos calcular o valor e atribuir ao polinomio
        char *infixa;
        polinomio res;

        infixa = malloc_safe(sizeof(char)*maxtam);
        scanf("%s", infixa);
        res = calcula_infixa(infixa);
        free(infixa);
        libera(var[p - 'a']);
        var[p - 'a'] = res;
        impr(p, var[p-'a']);
        descarta_resto_da_linha();
      }
      else if(op == 'u'){
        char c2,c3;

        scanf("%c%c", &c2, &c3);
        if(p == 'q' && c2 == 'i' && c3 == 't'){
          printf("Tchau!");
          for(i = 0; i<26; i++) libera(var[i]);
          return 0;
        } 
        else{
          printf("operacao desconhecida\n");
        }
        descarta_resto_da_linha();
      }
      else{
        printf("operacao desconhecida\n");
        descarta_resto_da_linha();
      }
    }
}