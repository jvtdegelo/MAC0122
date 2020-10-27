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
void imprime_menu();
int valido(int, int);
void calcula_distancia();
void acha_caminho();
void le_labirinto(char []);
void libera_matriz(int **);
int **declara_matriz();
void imprime_labirinto();
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
    //
    int **matriz;
    int i;

    matriz = malloc_safe(n * sizeof(int *));
    for(i=0; i<n; i++) matriz[i] = malloc_safe(m * sizeof(int));
    return matriz;
}

void libera_matriz(int **matriz){
    int i;
    
    if(n == -1) return;
    for(i=0; i<n;i++){
        free(matriz[i]);
    }
    free(matriz);
}

void imprime_menu(){
    printf("0: carregar um novo labirinto e posição inicial do Herbert\n");
    printf("1: dar nova posicao inicial do Herbert no mesmo labirinto\n");
    printf("2: sair do programa\n");
    printf("\nDigite a opcao desejada: ");
    return;
}

int valido(int x, int y){
    if(x < 0 || x >= n || y < 0 || y >= m) return 0;
    return 1;
}

void calcula_distancia(){
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
        printf("%d %d\n", x, y);
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

void le_labirinto(char arquivo[FNMAX]){
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

void imprime_caminho(){
    int i,j;

    if(distancia[x_ini][y_ini] == m*n + 1)
        printf("\n\nO Herbert não achou nenhuma cenoura!!! :(\n\n");
    else
        printf("\n\nO Herbert achou uma cenoura em %d passos!\n\n", distancia[x_ini][y_ini]);
    
    for(i=0; i<n; i++){
        for(j=0; j<m; j++){
            if(i==x_ini && j==y_ini) printf("H ");
            else if(distancia[i][j] == 0) printf("C ");
            else if(caminho[i][j] == -1) printf("* ");
            else printf("%d ", caminho[i][j]);
        }
        printf("\n\n");
    }
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
            printf("\n");
        }
        else if(op == 1){
            printf("\nDigite a posicao inicial do Herbert: ");
            scanf("%d %d", &x_ini, &y_ini);
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
