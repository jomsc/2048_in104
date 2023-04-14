#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int test(int x, int y) { return (x==y); }
int f(int x, int y) { return x+y; }


void combine(int i, char dir, int N)
{
    if (dir=='r') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=N-1;j2>=0;j2--) { if (A[i][j2]!=-1) { pos[j3]=j2; j3++; } }
        
        for (int k=0;k<n-1;k++) { 
            if (test(A[i][pos[k]],A[i][pos[k+1]])) {
                A[i][pos[k]] = f(A[i][pos[k]],A[i][pos[k+1]]);
                A[i][pos[k+1]] = -1;
                k++;
            }
        }

        free(pos);
    }

    if (dir=='l') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=0;j2<N;j++) { if (A[i][j2]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[i][pos[k]],A[i][pos[+1]])) {
                A[i][pos[k]] = f(A[i][pos[k]],A[i][pos[k+1]]);
                A[i][pos[k+1]] = -1;
                k++;
            }
        }

        free(pos);
    }

    if (dir=='u') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=0;j2<N;j++) { if (A[j2][i]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[pos[k]][i],A[pos[k+1]][i])) {
                A[pos[k]][i] = f(A[pos[k]][i],A[pos[k+1]][i])
                A[pos[k+1]][i] = -1;
                k++;
            }
        }

        free(pos);
    }

    if (dir=='d') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=N-1;j2>=0;j2--) { if (A[j2][i]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[pos[k]][i],A[pos[k+1]][i])) {
                A[pos[k]][i] = f(A[pos[k]][i],A[pos[k+1]][i])
                A[pos[k+1]][i] = -1;
                k++;
            }
        }

        free(pos);
    }
}

void collapse(int i, char dir, int N)
{
    int p=0;
    if (dir=='r')
    {
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=N-1;j>=0;j--) { if (A[i][j]!=-1) { L[j]=A[i][j]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[i][N-j-1]=L[j];  }
            else { A[i][N-j-1]=-1; }
        }

    }

    if (dir=='l')
    {
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { L[j]=A[i][j]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[i][j]=L[j];  }
            else { A[i][j]=-1; }
        }

    }

    if (dir=='d')
    {
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=N-1;j>=0;j--) { if (A[j][i]!=-1) { L[j]=A[j][i]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[N-j-1][i]=L[j];  }
            else { A[N-j-1][i]=-1; }
        }

    }

    if (dir=='u')
    {
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { L[j]=A[j][i]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[j][i]=L[j];  }
            else { A[j][i]=-1; }
        }

    }
}

void move(int N, char dir) {
    for (int i=0;i<=N;i++)
    {
        combine(i,dir,N);
        collapse(i,dir,N);
    }
}

void spawn(int N)
{
    int x = rand()%N;
    int y = rand()%N;

    while (A[x][y] != -1)
    {
        x = rand()%N;
        y = rand()%N;
    }

    int v=rand()%3;
    if (v==2) { A[x][y] = 4; }
    else { A[x][y] = 2; }
}

void display(int N)
{
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<N;j++)
        {
            printf("%d ",A[i][j]);
        }
        printf("\n");
    }
}

char get_input()
{
    char a;
    int read;
    read = scanf("%c",&a);
    return a;
}



int main() {
    srand(time(NULL));
    int N=4;
    int** A=malloc(sizeof(int)*N*N);

    char dir;

    for (int i=0;i<50;i++)
    {
        display(N);
        dir = get_input();
        move(N,dir);
        spawn(N);
    }

    return 0;
}
