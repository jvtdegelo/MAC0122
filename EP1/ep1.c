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

  ep1.c

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include<stdio.h>
#include<stdlib.h>

FILE *fp;


void curva_de_koch(double tam, int ordem){
    if(ordem == 0) {
        fprintf(fp, "%f 0 rlineto ", tam);
        return;
    }
    curva_de_koch(tam/3, ordem-1);
    fprintf(fp, "60 rotate ");

    curva_de_koch(tam/3, ordem-1);
    fprintf(fp, "240 rotate ");

    curva_de_koch(tam/3, ordem-1);
    fprintf(fp, "60 rotate ");

    curva_de_koch(tam/3, ordem-1);

}

void desenhar_H(double x, double y, double tam){
    fprintf(fp, "%f %f moveto ", x - tam/2, y - tam/2);

    fprintf(fp, "0 %f rlineto ",tam);

    fprintf(fp, "%f %f moveto ", x + tam/2, y - tam/2);

    fprintf(fp, "0 %f rlineto ",tam);

    fprintf(fp, "%f %f moveto ", x - tam/2, y);
    
    fprintf(fp, "%f 0 rlineto ",tam);
}

void arvore_H(double x, double y, double tam, int ordem){
    
    desenhar_H(x, y, tam);

    if(ordem == 0) return;
    
    arvore_H(x-tam/2, y-tam/2, tam/2, ordem-1);

    arvore_H(x+tam/2, y+tam/2, tam/2, ordem-1);
    
    arvore_H(x+tam/2, y-tam/2, tam/2, ordem-1);
    
    arvore_H(x-tam/2, y+tam/2, tam/2, ordem-1);
}

void desenhar_C(double x, double y, double tam, int sentido){
    fprintf(fp, "%f %f moveto ", x, y - tam/2);
    fprintf(fp, "0 %f rlineto ", tam);
    if(sentido == 0){
        fprintf(fp, "%f %f moveto ", x, y - tam/2);
        fprintf(fp, "%f 0 rlineto ", tam/2);

        fprintf(fp, "%f %f moveto ", x, y + tam/2);
        fprintf(fp, "%f 0 rlineto ", tam/2);
    }
    else{
        fprintf(fp, "%f %f moveto ", x - tam/2, y - tam/2);
        fprintf(fp, "%f 0 rlineto ", tam/2);

        fprintf(fp, "%f %f moveto ", x - tam/2, y + tam/2);
        fprintf(fp, "%f 0 rlineto ", tam/2);
    }
}

void arvore_C(double x, double y, double tam, int sentido, int ordem){
    // sentido representa o "lado" do C, 
    desenhar_C(x, y, tam, sentido);

    if(ordem == 0) return;

    if(sentido == 0){
        arvore_C(x + tam/2, y + tam/2, tam/2, 1-sentido, ordem-1);

        arvore_C(x + tam/2, y - tam/2, tam/2, 1-sentido, ordem-1);
    }
    else{
        arvore_C(x - tam/2, y + tam/2, tam/2, 1-sentido, ordem-1);

        arvore_C(x - tam/2, y - tam/2, tam/2, 1-sentido, ordem-1);
    }
}

int main(){
    int operacao;
    int ordem;
    char nomearq[200];

    scanf("%d %d", &operacao, &ordem);
    scanf(" %[^\n]", nomearq);

    if(operacao == 0){//estrela de koch
        double tam = 500;

        if(!( fp = fopen( nomearq, "w"))){
            printf("Erro ao abrir o arquivo %s\n\n", nomearq); 
            return 0;
        }

        fprintf(fp, "50 500 moveto ");
        
        curva_de_koch(tam, ordem);

        fprintf(fp ,"240 rotate ");

        curva_de_koch(tam, ordem);

        fprintf(fp ,"240 rotate ");

        curva_de_koch(tam, ordem);

        fprintf(fp,"stroke");
        fclose(fp);

    }

    else if(operacao == 1){//arvore_H
        double tam = 200;
        double x = 250, y= 500;


        if(!( fp = fopen( nomearq, "w"))){
            printf("Erro ao abrir o arquivo %s\n\n", nomearq); 
            return 0;
        }
        
        arvore_H(x, y, tam, ordem);

        fprintf(fp, "stroke");
        fclose(fp);
    }
    else if(operacao == 2){
        double x = 100, y = 400;
        double tam = 400;

        if(!( fp = fopen( nomearq, "w"))){
            printf("Erro ao abrir o arquivo %s\n\n", nomearq); 
            return 0;
        }

        arvore_C(x, y, tam, 0, ordem);

        fprintf(fp, "stroke");
        fclose(fp);
    }

}