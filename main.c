#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

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
    if (dir=='r') {
        for (int j=0;j<N;j++) {
            if (A[N*i+N-1-j]>0) {
                A[i*N+ N - 1 - p] = A[N * i + N - 1 - j];
                if (j!=p) { A[N * i + N - 1 - j] = -1; }
                p++;
            }
        }
    }

    if (dir=='l') {
        for (int j=0;j<N;j++) {
            if (A[N*i+j]>0) {
                A[i*N+p] = A[N * i + j];
                if (j!=p) { A[N * i + j] = -1; }
                p++;
            }
        }
    }

    if (dir=='u') {
        for (int j=0;j<N;j++) {
            if (A[N*j+i]>0) {
                A[N*p+i] = A[N*j+i];
                if (j!=p) { A[N*j+i] = -1; }
                p++;
            }
        }
    }

    if (dir=='d') {
        for (int j=0;j<N;j++) {
            if (A[i+N*(N-1-j)] > 0) {
                A[i+N*(N-1-p)] = A[i+N*(N-1-j)];
                if (j!=p) { A[i+N*(N-1-j)] = -1; }
                p++;
            }
        }
    }
}

void move(int N, char dir, int* A) {
    for (int i=0;i<N;i++)
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
            if (A[i*N+j]!=-1) { printf("%d|",A[i*N+j]); }
            else { printf(" |"); }
        }
        printf("\n");
    }
    printf("\n\n\n");
}


int main()
{
    system("clear");
    printf("\n\n\n\n\n\n\n\n\n");
    printf("                222222222222222           000000000             444444444        888888888\n");
    printf("                2:::::::::::::::22      00:::::::::00          4::::::::4      88:::::::::88\n");
    printf("                2::::::222222:::::2   00:::::::::::::00       4:::::::::4    88:::::::::::::88\n");
    printf("                2222222     2:::::2  0:::::::000:::::::0     4::::44::::4   8::::::88888::::::8\n");
    printf("                            2:::::2  0::::::0   0::::::0    4::::4 4::::4   8:::::8     8:::::8\n");
    printf("                            2:::::2  0:::::0     0:::::0   4::::4  4::::4   8:::::8     8:::::8\n");
    printf("                         2222::::2   0:::::0     0:::::0  4::::4   4::::4    8:::::88888:::::8\n");
    printf("                    22222::::::22    0:::::0     0:::::0 4::::444444::::444   8:::::::::::::8\n");
    printf("                  22::::::::222      0:::::0     0:::::0 4::::::::::::::::4  8:::::88888:::::8\n");
    printf("                 2:::::22222         0:::::0     0:::::0 4444444444:::::444 8:::::8     8:::::8\n");
    printf("                2:::::2              0:::::0     0:::::0           4::::4   8:::::8     8:::::8\n");
    printf("                2:::::2              0::::::0   0::::::0           4::::4   8:::::8     8:::::8\n");
    printf("                2:::::2       222222 0:::::::000:::::::0           4::::4   8::::::88888::::::8\n");
    printf("                2::::::2222222:::::2  00:::::::::::::00          44::::::44  88:::::::::::::88\n");
    printf("                2::::::::::::::::::2    00:::::::::00            4::::::::4    88:::::::::88\n");
    printf("                22222222222222222222      000000000              4444444444      888888888\n\n\n");
    printf("                                Par Clara Beaugrand et Joseph Mouscadet\n");
    sleep(5);
    system("clear");







    srand(time(NULL));
    int N=4;
    int* A=malloc(sizeof(int)*N*N);
    for (int i=0;i<N*N;i++) { A[i]=-1; }

    char n;
    char dir;
    system("stty -icanon");
    spawn(N,A);

    int playing=1;
    while(playing==1)
    {
        system("clear");
        printf("\n---------------------------------\n");
        for(int i=0;i<N;i++)
        {
            printf("|");
            for(int j=0;j<N;j++)
            {
                if(A[N*i+j]==-1)
                    printf("       |");
                else
                    printf("%5d  |",A[N*i+j]);
            }
            printf("\n---------------------------------\n");
        }
        printf("\n\nz,s,q,d-->up down left right ；m to end the game！\n");


        n=getchar();
        switch(n)
        {
            case 'z': //up
                dir='u';
                break;
            case 's': //down
                dir='d';
                break;
            case 'q': //left
                dir='l';
                break;
            case 'd': //right
                dir='r';
                break;
            case 'm': //ESC
                printf("\n Exit Game ！\n");
                playing=0;
                break;
            default:
                dir=' ';
                break;
        }

        move(N,dir,A);
        spawn(N,A);

        //win or not
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                if(A[N*i+j]==2048)//2048 win
                {
                    printf("\n You win！\n");
                    return 1;//end
                }
            }
        }
    }

    return 0;
}

