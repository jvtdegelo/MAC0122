#include<stdio.h>
#include<stdlib.h>
#include "polinomios.h"

void *mallocSafe (size_t);
polinomio insere(float, int);
polinomio mult_por_mon(polinomio, float, int);

struct monomio{
    double coef;
    int exp;
    struct monomio *prox;
};

typedef struct monomio Monomio; 

void *mallocSafe (size_t nbytes){
   void *ptr;

   ptr = malloc (nbytes);
   if (ptr == NULL) {
      printf ("Socorro! malloc devolveu NULL!\n");
      exit (EXIT_FAILURE);
   }
   return ptr;
}

void impr(char c, polinomio p){
    //funcao que imprime um polinomio

    Monomio *i;

    printf("%c(x) = ",c);
    if(p == NULL) printf("0");
    for(i = p; i != NULL; i = i->prox)
    {
        if(i==p) printf("%.2fx^%d", i->coef, i->exp);// se for o primeiro nao imprimimos o sinal de +
        else printf(" + %.2fx^%d", i->coef, i->exp);
    }
    printf("\n\n");
}

polinomio cria(){
    //nao estamos usando cabeca de lista, entao nosso polinomio nulo é apenas um ponteiro para NULL
    return NULL;
}

polinomio copia(polinomio p){
    //nese vamos passar pelo polinomio p, 
    //criando um identico a ele

    Monomio *q = cria();// armazena o endereco do primeiro monomio
    Monomio *i, *j;//i vai iterar por p e j na copia

    if(p != NULL) q = mallocSafe(sizeof(Monomio));// ja criamos o primeiro monomio
    j = q;
    for(i = p; i !=NULL; i = i->prox){
        if(i->prox != NULL) j->prox = mallocSafe(sizeof(Monomio));
        // se existir o proximo ja criamos ele e fazemos o j apontar para ele

        else j -> prox = NULL;//se nao, encerramos a lista
        //copiamos p e atualizamos o j
        j->coef = i->coef;
        j->exp = i->exp; 
        j = j->prox;
    }
    //retornamos o apontador para o primeiro monomio
    return (void *)q;
}

polinomio leia(){
    //nesse caso lemos o expoente e o coeficiente e o adicionamos a lista
    int expoente;
    float coeficiente;
    Monomio *pr;
    Monomio *ini = NULL;//lista vazia
    
    while(1){
        scanf("%f", &coeficiente);
        if(coeficiente == 0) break;
        scanf("%d", &expoente);

        if(ini == NULL || expoente > ini->exp){
            pr = ini;
            ini = insere(coeficiente, expoente);
            ini->prox = pr;
        }
        else{
            Monomio *i = ini;
            while(i->prox != NULL && !(i->exp > expoente && (i->prox)->exp < expoente)) i = i->prox;
            pr = i->prox;
            i->prox = insere(coeficiente, expoente);
            (i->prox)-> prox = pr;
        }
    }
    return (void *) ini;
}

polinomio insere(float coeficiente, int expoente){//funcao que aloca memoria para um novo monomio e retorna seu endereco
    Monomio *atual;

    atual = mallocSafe(sizeof(Monomio));

    atual -> coef = coeficiente;
    atual -> exp = expoente;

    return atual;
}

polinomio soma(polinomio p, polinomio q){
    //nessa funcao, mantemos um apontador P de p, e Q de q. A cada momento vemos se
    //1)P é NULL, nesse caso devemos adicionar o monomio que Q aponta
    //2)Q é NULL, nesse caso devemos adicionar o monomio que P aponta 
    //3)P->exp > Q->exp, para manter a ordem decrescente devemos adicionar o monomio de P
    //4)P->exp < Q->exp, para manter a ordem decrescente devemos adicionar o monomio de Q
    //5)os expoentes sao iguais, nesse caso devemos adicionar um monomio de igual expoente e com a soma dos coeficientes, caso essa soma seja diferente de 0
    // fazemos desse jeito para que precisemos passar apenas uma vez por cada polinomio
    Monomio *P,*Q;
    Monomio *s = NULL;//aponta para o primeiro monomio
    Monomio *s_at = NULL, *s_ant = NULL;

    P = p;
    Q = q;
    while (!(Q==NULL && P==NULL)){
        if(P == NULL){
            s_at = insere (Q -> coef, Q -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at; // nao é o primeiro a ser inserido
            else s = s_at;//é o primeiro a ser inserido
            s_ant = s_at;
            Q = Q -> prox;
        }
        else if(Q == NULL){
            s_at = insere(P -> coef, P -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at; 
            else s = s_at;
            s_ant = s_at;
            P = P -> prox;
        }
        else if(P -> exp > Q -> exp){
            s_at = insere(P -> coef, P -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at; 
            else s = s_at;
            s_ant = s_at;
            P = P -> prox;
        }
        else if(P -> exp < Q -> exp){
            s_at = insere(Q -> coef, Q -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at; 
            else s = s_at;
            s_ant = s_at;
            Q = Q -> prox;
        }
        else{
            if(!((P -> coef) + (Q -> coef) < 0.000001 && (P -> coef) + (Q -> coef) > -0.000001)){// se a soma nao for zero
                s_at = insere((P -> coef) + (Q -> coef), P -> exp);
                if(s_ant != NULL) s_ant -> prox = s_at;
                else s = s_at;
                s_ant = s_at;
            }
            P = P -> prox;
            Q = Q -> prox;
        }
    }
    if(s_ant != NULL) s_ant -> prox = NULL;
    return (void *) s;
}

polinomio subt(polinomio p, polinomio q){
    //nessa funcao, mantemos um apontador P de p, e Q de q. A cada momento vemos se
    //1)P é NULL, nesse caso devemos adicionar o monomio que Q aponta(negativo)
    //2)Q é NULL, nesse caso devemos adicionar o monomio que P aponta 
    //3)P->exp > Q->exp, para manter a ordem decrescente devemos adicionar o monomio de P
    //4)P->exp < Q->exp, para manter a ordem decrescente devemos adicionar o monomio de Q(negativo)
    //5)os expoentes sao iguais, nesse caso devemos adicionar um monomio de igual expoente e com a diferenca dos coeficientes, caso ela seja diferente de 0
    // fazemos desse jeito para que precisemos passar apenas uma vez por cada polinomio
    Monomio *P,*Q;
    Monomio *s = NULL;
    Monomio *s_at = NULL, *s_ant = NULL;

    P = p;
    Q = q;
    while(!(Q==NULL && P==NULL)){
        if(P == NULL){
            s_at = insere(- Q -> coef, Q -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at; 
            else s = s_at;
            s_ant = s_at;
            Q = Q -> prox;
        }
        else if(Q == NULL){
            s_at = insere(P -> coef, P -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at;
            else s = s_at;
            s_ant = s_at;
            P = P -> prox;
        }
        else if(P -> exp > Q -> exp){
            s_at = insere(P -> coef, P -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at;
            else s = s_at;
            s_ant = s_at;
            P = P -> prox;
        }
        else if(P -> exp < Q -> exp){
            s_at = insere(- Q -> coef, Q -> exp);
            if(s_ant != NULL) s_ant -> prox = s_at;
            else s = s_at;
            s_ant = s_at;
            Q = Q -> prox;
        }
        else{
            if(!((P -> coef) - (Q -> coef) < 0.00001 && (P -> coef) - (Q -> coef) > -0.00001)){
                s_at = insere((P -> coef) - (Q -> coef), P -> exp);
                if(s_ant != NULL) s_ant -> prox = s_at;
                else s = s_at;
                s_ant = s_at;
            }
            P = P -> prox;
            Q = Q -> prox;
        }
    }
    if(s_ant != NULL) s_ant -> prox = NULL;
    return (void *) s;
}

polinomio nega(polinomio p){
    //copiamos p e modificamos a copia
    Monomio *np = copia(p);
    Monomio *i;

    for(i=np; i != NULL; i = i-> prox){
        i -> coef = -i -> coef;
    }
    return (void *)np;
}

void libera(polinomio p){
    //guardamos o indice do proximo antes de liberar o atual
    Monomio *i = p, *pr;

    while (i!=NULL){
        pr = i -> prox;
        free(i);
        i = pr; 
    }
}

polinomio mult(polinomio p, polinomio q){
    //nessa funcao, multiplicamos q por todos os monomios de p (quando multiplicamos por um monomio eh facil de manter a ordem)
    //e vamos somando (que tambem mantem a ordem)
    Monomio *i, *s = NULL, *copia_s, *nq;
    if(p == NULL|| q== NULL) return (void *)NULL;
    for(i=p; i!=NULL; i = i->prox){
        nq = mult_por_mon(q, i -> coef, i -> exp);
        copia_s = s;//copiamos s para nao perder a posicao que estava
        s = soma(copia_s, nq);//s recebe a soma
        //liberamos as outras variaveis usadas
        libera(copia_s);
        libera(nq); 
    }
    return (void *)s;
}

polinomio quoc(polinomio p, polinomio q){// vamos simular o algoritmo de divisao de polinomios
    Monomio *i = copia(p), *j = q;
    Monomio *q_ini= NULL, *q_at, *q_ant = NULL;

    if(p == NULL) return NULL;
    if(q == NULL){
        printf("operacao indefinida");
        return NULL;
    }
    while(i!=NULL && i->exp >= j->exp){
        float coeficiente = i->coef/j->coef;
        int expoente = i->exp-j->exp;
        Monomio *sub = mult_por_mon(j, coeficiente, expoente);
        Monomio *ni = i;
        i = subt(i, sub);
        libera(sub);
        libera(ni);
        if(q_ini == NULL){
            q_ini = insere(coeficiente, expoente);
            q_ant = q_ini;
        }

        else{
            q_at = insere(coeficiente, expoente);
            q_ant-> prox = q_at;
            q_ant = q_at;
        }
    }
    if(q_ini != NULL) q_ant->prox = NULL;
    libera(i);
    return (void *) q_ini;
}

polinomio rest(polinomio p, polinomio q){
    //muito parecido com a divisao, mas nao precisamos manter o quoc em uma lista encadeada
    Monomio *i = copia(p), *j = q;

    if(p == NULL) return NULL;
    if(q == NULL){
        printf("operacao indefinida");
        return NULL;
    }
    while(i!=NULL && i->exp >= j->exp){
        float coeficiente = i->coef/j->coef;
        int expoente = i->exp-j->exp;
        Monomio *sub = mult_por_mon(j, coeficiente, expoente);
        Monomio *ni = i;
        i = subt(i, sub);
        libera(sub);
        libera(ni);
    }
    return (void *) i;
}

polinomio mult_por_mon(polinomio p, float coeficiente, int expoente){
    // funcao que retorna um polinomio multiplicado por um monomio
    Monomio *ini=NULL, *i, *j_ant = NULL, *j;
    for(i=p; i!=NULL;i = i -> prox){
        if(j_ant == NULL){
            ini = insere(coeficiente*(i->coef), expoente + (i->exp));
            j_ant = ini;
        }
        else{
            j = insere(coeficiente*(i->coef), expoente + (i->exp));
            j_ant->prox = j;
            j_ant = j;
        }
    }
    j_ant -> prox = NULL;

    return (void *)ini;
}
