#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

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
            if (A[N*i+N-1-j]!=-1) {
                A[i*N+ N - 1 - p] = A[N * i + N - 1 - j];
                if (j!=p) { A[N * i + N - 1 - j] = -1; }
                p++;
            }
        }
    }

    if (dir=='l') {
        for (int j=0;j<N;j++) {
            if (A[N*i+j]!=-1) {
                A[i*N+p] = A[N * i + j];
                if (j!=p) { A[N * i + j] = -1; }
                p++;
            }
        }
    }

    if (dir=='u') {
        for (int j=0;j<N;j++) {
            if (A[N*j+i]!=-1) {
                A[N*p+i] = A[N*j+i];
                if (j!=p) { A[N*j+i] = -1; }
                p++;
            }
        }
    }

    if (dir=='d') {
        for (int j=0;j<N;j++) {
            if (A[i+N*(N-1-j)]!=-1) {
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

    int v=rand()%6;
    if (v==5) { A[x*N+y] = 4; }
    else { A[x*N+y] = 2; }
}


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "2048!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}
SDL_Texture* loadTexture(char *path) {
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);

    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }

    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load PNG texture
    gTexture = loadTexture( "/Users/phesox/CLionProjects/2048_in104/assets/textures/rect.png" );
    if( gTexture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }

    return success;
}
void wclose()
{
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer( gRenderer);
    SDL_DestroyWindow( gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    gTexture = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}



int main()
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            bool quit = false;
            SDL_Event e;

            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                SDL_SetRenderDrawColor( gRenderer, 0x80, 0x80, 0x80, 0xFF );
                SDL_RenderClear(gRenderer);

                SDL_Rect textureRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

                SDL_RenderCopy(gRenderer, gTexture, NULL, &textureRect);

                SDL_RenderPresent(gRenderer);
            }
        }
    }



    /*
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

        for(int i=0;i<N*N;i++)
        {
            if(A[i]==2048) {
                printf("\n You win！\n");
                return 1;//end
            }
        }
    }*/

    wclose();

    return 0;
}