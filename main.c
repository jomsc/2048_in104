#include <stdio.h>
#include <time.h>
#include <stdlib.h>

bool test(int x, int y) { return (x==y); }
int f(int x, int y) { return x+y; }


void move(int N) {

    for (int i=0;i<=N;i++)
    {
        combine(i,'r');
        collapse(i,'r');
    }
}

void combine(int i, char dir)
{
    if (dir=='r') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i][j]!=-1) { n++; } }
        int pos[n];
        int j3=0;
        for (int j2=N-1;j2<0;j2--) { if (A[i][j2]!=-1) { pos[j3]=j2; j3++; } }
        
        for (int k=0;k<n-1;k++) { 
            if (test(A[i][pos[k]],A[i][pos[k+1]])) {
                A[i][pos[k]] = f(A[i][pos[k]],A[i][pos[k+1]]);
                A[i][pos[k+1]] = -1;
                k++;
            }
        }
    }

    if (dir=='g') {
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

    if (dir=='u') 

}











int main() {
    srand(time(NULL));
    int N=4;
    int** A=malloc(sizeof(int)*N*N);

    

    return 0;
}
