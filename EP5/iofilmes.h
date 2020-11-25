/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO

  NO ENTANTO, VOCE PODE ACRESCENTAR ALGO QUE ACHAR CONVENIENTE
  COMO PROTOTIPOS DE NOVAS FUNCOES.

  Aqui estao os prototipo das funcoes de entrada e saida 
  de filmes.
  
*/

#ifndef _IOFILMES_H
#define _IOFILMES_H

#include "util.h"   /* Bool */
#include "filmes.h" /* ListaFilmes, Filme */

void
carregueListaFilmes(ListaFilmes *lst, Bool semRepeticoes);

void
graveListaFilmes(ListaFilmes *lst);

void
mostreFilme(Filme *flm);

void
mostreListaFilmes(ListaFilmes *lst);

void
mostrePioresFilmes(ListaFilmes *lst);

void
mostreMelhoresFilmes(ListaFilmes *lst);

#endif
