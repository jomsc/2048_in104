#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int test(int x, int y) { return (x==y); }
int f(int x, int y) { return x+y; }


void combine(int i, char dir, int N, int* A)
{
    if (dir=='r') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i*N+j]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=N-1;j2>=0;j2--) { if (A[i*N+j2]!=-1) { pos[j3]=j2; j3++; } }
        
        for (int k=0;k<n-1;k++) { 
            if (test(A[i*N+pos[k]],A[i*N+pos[k+1]])) {
                A[i*N+pos[k]] = f(A[i*N+pos[k]],A[i*N+pos[k+1]]);
                A[i*N+pos[k+1]] = -1;
                k++;
            }
        }

        free(pos);
    }

    if (dir=='l') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i*N+j]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=0;j2<N;j2++) { if (A[i*N+j2]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[i*N+pos[k]],A[i*N+pos[k+1]])) {
                A[i*N+pos[k]] = f(A[i*N+pos[k]],A[i*N+pos[k+1]]);
                A[i*N+pos[k+1]] = -1;
                k++;
            }
        }

        free(pos);
    }

    if (dir=='u') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[j*N+i]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=0;j2<N;j2++) { if (A[j2*N+i]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[pos[k]*N+i],A[pos[k+1]*N+i])) {
                A[pos[k]*N+i] = f(A[pos[k]*N+i],A[pos[k+1]*N+i]);
                A[pos[k+1]*N+i] = -1;
                k++;
            }
        }

        free(pos);
    }

    if (dir=='d') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[j*N+i]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=N-1;j2>=0;j2--) { if (A[j2*N+i]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[pos[k]*N+i],A[pos[k+1]*N+i])) {
                A[pos[k]*N+i] = f(A[pos[k]*N+i],A[pos[k+1]*N+i]);
                A[pos[k+1]*N+i] = -1;
                k++;
            }
        }

        free(pos);
    }
}

void collapse(int i, char dir, int N, int* A)
{
    int p=0;
    if (dir=='r')
    {
        for (int j=0;j<N;j++) { if (A[i*N+j]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=N-1;j>=0;j--) { if (A[i*N+j]!=-1) { L[j]=A[i*N+j]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[i*N+N-j-1]=L[j];  }
            else { A[i*N+N-j-1]=-1; }
        }

    }

    if (dir=='l')
    {
        for (int j=0;j<N;j++) { if (A[i*N+j]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=0;j<N;j++) { if (A[i*N+j]!=-1) { L[j]=A[i*N+j]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[i*N+j]=L[j];  }
            else { A[i*N+j]=-1; }
        }

    }

    if (dir=='d')
    {
        for (int j=0;j<N;j++) { if (A[j*N+i]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=N-1;j>=0;j--) { if (A[j*N+i]!=-1) { L[j]=A[j*N+i]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[(N-j-1)*N+i]=L[j];  }
            else { A[(N-j-1)*N+i]=-1; }
        }

    }

    if (dir=='u')
    {
        for (int j=0;j<N;j++) { if (A[j*N+i]!=-1) { p++; } }
        int* L=malloc(sizeof(int)*p);
        for (int j=0;j<N;j++) { if (A[j*N+i]!=-1) { L[j]=A[j*N+i]; } }
        for (int j=0;j<N;j++)
        {
            if (j<p) {  A[j*N+i]=L[j];  }
            else { A[j*N+i]=-1; }
        }

    }
}

void move(int N, char dir, int* A) {
    for (int i=0;i<=N;i++)
    {
        combine(i,dir,N,A);
        collapse(i,dir,N,A);
    }
}

void spawn(int N, int* A)
{
    int x = rand()%N;
    int y = rand()%N;

    while (A[x*N+y] != -1)
    {
        x = rand()%N;
        y = rand()%N;
    }

    int v=rand()%3;
    if (v==2) { A[x*N+y] = 4; }
    else { A[x*N+y] = 2; }
}

void display(int N, int* A)
{
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<N;j++)
        {
            if (A[i*N+j]!=-1) { printf("%d ",A[i*N+j]); }
            else { printf("  "); }
        }
        printf("\n");
    }
    printf("\n\n\n");
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
    int* A=malloc(sizeof(int)*N*N);


    char dir;

    for (int i=0;i<50;i++)
    {
        display(N, A);
        dir = get_input();
        move(N,dir,A);
        spawn(N,A);
    }

    free(A);
    return 0;
}

