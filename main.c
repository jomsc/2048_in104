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
        int pos[n];
        int j3=0;
        for (int j2=N-1;j2>0;j2--) { if (A[i][j2]!=-1) { pos[j3]=j2; j3++; } }
        
        for (int k=0;k<n-1;k++) { 
            if (test(A[i][pos[k]],A[i][pos[k+1]])) {
                A[i][pos[k]] = f(A[i][pos[k]],A[i][pos[k+1]]);
                A[i][pos[k+1]] = -1;
                k++;
            }
        }
    }

    if (dir=='l') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { n++; } }
        int pos[n];
        int j3=0;
        for (int j2=0;j2<N;j++) { if (A[i][j2]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[i][pos[k]],A[i][pos[+1]])) {
                A[i][pos[k]] = f(A[i][pos[k]],A[i][pos[k+1]]);
                A[i][pos[k+1]] = -1;
                k++;
            }
        }
    }

    if (dir=='u') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { n++; } }
        int pos[n];
        int j3=0;
        for (int j2=0;j2<N;j++) { if (A[j2][i]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[pos[k]][i],A[pos[k+1]][i])) {
                A[pos[k]][i] = f(A[pos[k]][i],A[pos[k+1]][i])
                A[pos[k+1]][i] = -1;
                k++;
            }
        }
    }

    if (dir=='d') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[j][i]!=-1) { n++; } }
        int pos[n];
        int j3=0;
        for (int j2=N-1;j2>0;j2--) { if (A[j2][i]!=-1) { pos[j3]=j2; j3++; } }

        for (int k=0;k<n-1;k++) {
            if (test(A[pos[k]][i],A[pos[k+1]][i])) {
                A[pos[k]][i] = f(A[pos[k]][i],A[pos[k+1]][i])
                A[pos[k+1]][i] = -1;
                k++;
            }
        }
    }
}

void collapse(int i, char dir, int N)
{
    printf("test");
}

void move(int N, char dir) {
    for (int i=0;i<=N;i++)
    {
        combine(i,dir,N);
        collapse(i,dir,N);
    }
}









int main() {
    int N=4;
    int** A=malloc(sizeof(int)*N*N);

    

    return 0;
}
