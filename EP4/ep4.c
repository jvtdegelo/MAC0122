#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "queue.h"

#define FNMAX 200

int **labirinto, **distancia, **caminho;
int *cenoura_x, *cenoura_y;
int cenouras;
int n=-1, m;
int x_ini, y_ini;
int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1}; 

void *malloc_safe(size_t);
int **declara_matriz();
void libera_matriz(int **);
void imprime_menu();
int valido(int, int);
void le_labirinto(char []);
void imprime_labirinto();
void calcula_distancia();
void acha_caminho();
void imprime_caminho();

void *malloc_safe (size_t nbytes){
   void *ptr;

   ptr = malloc (nbytes);
   if (ptr == NULL) {
      printf ("Socorro! malloc devolveu NULL!\n");
      exit (EXIT_FAILURE);
   }
   return ptr;
}

int **declara_matriz(){
    //funcao que declara uma matriz de 
    //dimensao n x m, que estao declarados globalmente
    int **matriz;
    int i;

    matriz = malloc_safe(n * sizeof(int *));
    for(i=0; i<n; i++) matriz[i] = malloc_safe(m * sizeof(int));
    return matriz;
}

void libera_matriz(int **matriz){
    //funcao que libera a matriz de dimensao n x m
    int i;
    
    if(n == -1) return;
    for(i=0; i<n;i++){
        free(matriz[i]);
    }
    free(matriz);
}

void imprime_menu(){
    //funcao que imprime o menu
    printf("0: carregar um novo labirinto e posição inicial do Herbert\n");
    printf("1: dar nova posicao inicial do Herbert no mesmo labirinto\n");
    printf("2: sair do programa\n");
    printf("\nDigite a opcao desejada: ");
    return;
}

int valido(int x, int y){
    //verifica se a posicao esta no labirinto
    if(x < 0 || x >= n || y < 0 || y >= m) return 0;
    return 1;
}

void le_labirinto(char arquivo[FNMAX]){
    //funcao que le um labirinto no arquivo passado
    //e atualiza as variaveis globais n, m, labirinto
    //cenouras, etc
    FILE *fp;
    int i,j;
    
    libera_matriz(labirinto);
    free(cenoura_x);
    free(cenoura_y);
    if(! ( fp = fopen( arquivo, "r"))){
        printf("Erro na abertura do arquivo %s\n\n ERRO: Operacao abortada devido a erro na leitura\n\n\n",arquivo);
        return;
    }
    fscanf(fp, "%d %d", &n, &m);
    labirinto = declara_matriz();
    for(i=0; i<n;i++)
        for(j=0; j<m; j++)
            fscanf(fp, "%d", &labirinto[i][j]);
    fscanf(fp, "%d", &cenouras);
    cenoura_x = malloc_safe(cenouras * sizeof(int));
    cenoura_y = malloc_safe(cenouras * sizeof(int));
    for(i=0; i<n; i++) fscanf(fp, "%d %d", &cenoura_x[i], &cenoura_y[i]);
    for(i=0; i<n; i++){
        cenoura_x[i]--;
        cenoura_y[i]--;
    }
    fclose(fp);
}

void imprime_labirinto(){
    //funcao que imprime o labirinto 
    int i,j;

    printf("Labirinto:\n");
    for(i=0; i<n; i++){
        for (j=0; j<m;j ++){
            printf("%d ", labirinto[i][j]);
        }
        printf("\n");
    }
    return;
}

void calcula_distancia(){
    //funcao que calcula a distancia de todas as casas
    //do labirinto ate uma cenoura, fazemos isso setando
    //as distancias ate todas as cenouras para 0 e 
    //adicionando todas as posicoes com cenoura a pilha
    int i,j;
    int inf = n*m +1;
    Queue qx = queueInit(n * m * sizeof(int));
    Queue qy = queueInit(n * m * sizeof(int));

    distancia = declara_matriz();
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            distancia[i][j] = inf;
    for(i = 0; i < cenouras; i++){
        distancia[cenoura_x[i]][cenoura_y[i]] = 0;
        queuePut(qx, cenoura_x[i]);
        queuePut(qy, cenoura_y[i]);
    }
    while(!queueEmpty(qx)){
        int x,y;
        int d;

        x = queueGet(qx);
        y = queueGet(qy);
        d = distancia [x][y];
        for(i=0; i<4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(valido(nx,ny) && labirinto[nx][ny] == 0 && distancia[nx][ny] == inf){
                distancia[nx][ny] = d + 1;
                queuePut(qx, nx);
                queuePut(qy, ny);
            }
        }
    }
    queueFree(qx);
    queueFree(qy);
}

void acha_caminho(){
    //funcao que com as distancias calculadas
    //determina um caminho do herbert ate uma cenoura
    //fazemos isso partindo do herbert e checando de que posicao
    //que posicao ele veio (dist[veio]+1 = distancia[atual])
    //caso haja dois vizinhos que satisfacam isso, escolhemos
    //qualque uma
    int x, y;
    int i,j;

    caminho = declara_matriz();
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            caminho[i][j] = labirinto[i][j];
    if(distancia[x_ini][y_ini] == n*m + 1) return;
    x = x_ini;
    y = y_ini;

    while(distancia[x][y] != 0){
        for(i=0; i<4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(valido(nx, ny) && distancia[nx][ny] + 1 == distancia[x][y]){
                caminho[nx][ny] = -1;
                x = nx;
                y = ny;
                break;
            }
        }
    }
}



void imprime_caminho(){
    //funcao que imprime o caminho
    int i,j;

    if(distancia[x_ini][y_ini] == m*n + 1)
        printf("\nO Herbert não achou nenhuma cenoura!!! :(\n\n");
    else
        printf("\nO Herbert achou uma cenoura em %d passos!\n\n", distancia[x_ini][y_ini]);
    
    for(i=0; i<n; i++){
        for(j=0; j<m; j++){
            if(i==x_ini && j==y_ini) printf("H ");
            else if(distancia[i][j] == 0) printf("C ");
            else if(caminho[i][j] == -1) printf("* ");
            else printf("%d ", caminho[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    int op;
    char arquivo[FNMAX];

    printf("*************************\nCarrotsvile search engine\n*************************\n\n");
    while(1){
        imprime_menu();
        scanf("%d", &op);
        if(op == 0){
            printf("\nDigite o nome do arquivo com o labirinto: ");
            scanf(" %[^\n]", arquivo);
            printf("\n");
            le_labirinto(arquivo);
            imprime_labirinto();
            printf("\nDigite a posicao inicial do Herbert: ");
            scanf(" %d %d", &x_ini, &y_ini);
            x_ini--;
            y_ini--;
            calcula_distancia();
            acha_caminho();
            imprime_caminho();
            libera_matriz(distancia);
            libera_matriz(caminho);
        }
        else if(op == 1){
            printf("\nDigite a posicao inicial do Herbert: ");
            scanf(" %d %d", &x_ini, &y_ini);
            x_ini--;
            y_ini--;
            calcula_distancia();
            acha_caminho();
            imprime_caminho();
            libera_matriz(distancia);
            libera_matriz(caminho);
        }
        else{
            libera_matriz(labirinto);
            free(cenoura_x);
            free(cenoura_y);
            printf("\nTchau, Herbert!");
            return 0;
        }
    }
}
