#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

enum status { MENU, CLASSIC, QUANTIC, DYNAMIC, NDIM, SETTINGS, VICTORY };

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
    for (int i=0;i<N;i++) {
        combine(i,dir,N,A);
        collapse(i,dir,N,A);
    }
}
void spawn(int N, int* A)
{
    int a=0;
    for (int i=0;i<N*N;i++) { if (A[i] != -1) { a++; }}
    if (a<N*N) {
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
}

bool isInside(int x, int y, int w, int h, int a, int b) {
    return ((x<=(a+w) && x>=a) && (y<=(b+h) && y>= b));
}

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Texture* classicBackgroundTexture = NULL;
SDL_Texture* resetButton = NULL;
SDL_Texture* texture2 = NULL;
SDL_Texture* texture4 = NULL;
SDL_Texture* texture8 = NULL;
SDL_Texture* texture16 = NULL;
SDL_Texture* texture32 = NULL;
SDL_Texture* texture64 = NULL;
SDL_Texture* texture128 = NULL;
SDL_Texture* texture256 = NULL;
SDL_Texture* texture512 = NULL;
SDL_Texture* texture1024 = NULL;
SDL_Texture* texture2048 = NULL;
SDL_Texture* rectTexture = NULL;

bool isInitialClassic = true;


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
bool loadMedia_rect()
{
    //Loading success flag
    bool success = true;

    //Load PNG texture
    rectTexture = loadTexture( "/Users/phesox/CLionProjects/2048_in104/assets/textures/rect.png" );
    if( rectTexture == NULL )
    {
        printf( "Failed to load rectangle texture!\n" );
        success = false;
    }

    return success;
}
bool loadMedia_number(int number, SDL_Texture** texture)
{
    //Loading success flag
    bool success = true;
    char textureText[20];
    strcpy(textureText, "");
    if (number != -1) { sprintf(textureText, "%d", number); }

    char path[100];
    strcpy(path,"");
    strcat(path, "/Users/phesox/CLionProjects/2048_in104/assets/textures/numbers/");
    strcat(path,textureText);
    strcat(path,".png");
    printf("%s\n",path);
    //Load PNG texture
    *texture = loadTexture(path);
    if( *texture == NULL )
    {
        printf( "Failed to load number %d texture !\n", number);
        success = false;
    }

    return success;
}
bool loadMedia(char* path, SDL_Texture** texture) {
    bool success = true;
    *texture = loadTexture(path);
    if( *texture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }
    return success;
}
bool textureInit() {
    loadMedia_rect();
    loadMedia_number(2,&texture2);
    loadMedia_number(4,&texture4);
    loadMedia_number(8,&texture8);
    loadMedia_number(16,&texture16);
    loadMedia_number(32,&texture32);
    loadMedia_number(64,&texture64);
    loadMedia_number(128,&texture128);
    loadMedia_number(256,&texture256);
    loadMedia_number(512,&texture512);
    loadMedia_number(1024,&texture1024);
    loadMedia_number(2048,&texture2048);

    if (!loadMedia("/Users/phesox/CLionProjects/2048_in104/assets/textures/background.png", &classicBackgroundTexture)) {
        printf("Failed to load game background!");
    }

    if (!loadMedia("/Users/phesox/CLionProjects/2048_in104/assets/textures/reset.png", &resetButton)) {
        printf("Failed to load reset button!");
    }

    return true;
}
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

        textureInit();
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

    IMG_Quit();
    SDL_Quit();
}

void game_display_classic(int* A, int N) {
    isInitialClassic = false;
    SDL_Texture* numberTextures[] = {texture2, texture4, texture8, texture16, texture32,
                                     texture64, texture128, texture256, texture512,texture1024,
                                     texture2048 };
    int margin = 20;
    int rect_size = 100;
    int offset = margin;
    int* posx = malloc(sizeof(int)*N*N);
    int* posy = malloc(sizeof(int)*N*N);

    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            posy[N*i+j] = offset+margin+(rect_size+margin)*i;
            posx[N*i+j] = offset+margin+(rect_size+margin)*j;
        }
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    SDL_Rect backgroundRect = { offset, offset, N*rect_size+(N+1)*margin, N*rect_size+(N+1)*margin};
    SDL_RenderCopy(gRenderer, classicBackgroundTexture, NULL, &backgroundRect);

    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            int number = A[N*i+j];

            if (number == -1) {
                SDL_Rect textureRect = {posx[N * i + j], posy[N * i + j], rect_size, rect_size};
                SDL_RenderCopy(gRenderer, rectTexture, NULL, &textureRect);
            }
            else {
                int a = (int) (log(number)/log(2)-1);
                SDL_Rect textureRect_text = {posx[N * i + j], posy[N * i + j], rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numberTextures[a], NULL, &textureRect_text);
            }
        }
    }

    SDL_RenderPresent(gRenderer);

    free(posx);
    free(posy);
}
void main_menu_display(SDL_Event e, enum status *status, int* A[], int N) {
    int nb_buttons=3;
    int initialMargin = 100;
    int margin = 20;
    int wButton = 256;
    int hButton = 64;
    int* posx = malloc(sizeof(int)*nb_buttons);
    int* posy = malloc(sizeof(int)*nb_buttons);

    for (int i=0;i<nb_buttons;i++) {
        posy[i] = initialMargin+(hButton+margin)*i;
        posx[i] = SCREEN_WIDTH/2-wButton/2;
    }
    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    if(!loadMedia("/Users/phesox/CLionProjects/2048_in104/assets/textures/play_classic.png", &gTexture)) {
        printf("Failed to load classic button!\n");
    } else {
        SDL_Rect textureRect = { posx[0], posy[0], wButton, hButton};
        SDL_RenderCopy(gRenderer, gTexture, NULL, &textureRect);
        if (!isInitialClassic) {
            SDL_Rect resetRect = { posx[0]+wButton+margin, posy[0], hButton, hButton};
            SDL_RenderCopy(gRenderer, resetButton, NULL, &resetRect);
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x,y;
        SDL_GetMouseState(&x, &y);
        if (isInside(x,y,wButton,hButton,posx[0],posy[0])) { *status = CLASSIC; }
        if (isInside(x,y,hButton,hButton,posx[0]+wButton+margin,posy[0]) && !isInitialClassic) {
            isInitialClassic = true;
            for (int k=0;k<(N*N);k++) {
                (*A)[k]=-1;
                printf("%d", k);
            }
            spawn(N, *A);
        }
    }

    SDL_RenderPresent(gRenderer);
}



int main()
{
    enum status status = MENU;

    srand(time(NULL));
    int N=4;
    int* A=malloc(sizeof(int)*N*N);
    for (int i=0;i<N*N;i++) { A[i]=-1; }
    char dir;
    spawn(N,A);

    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {

        bool quit = false;
        SDL_Event e;
        while( !quit )
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                if (status==CLASSIC) {
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    game_display_classic(A, N);
                    if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_z:
                                dir='u';
                                break;
                            case SDLK_s:
                                dir='d';
                                break;
                            case SDLK_q:
                                dir='l';
                                break;
                            case SDLK_d:
                                dir='r';
                                break;
                            case SDLK_ESCAPE:
                                status=MENU;
                                dir='z';
                                break;
                            default:
                                dir='x';
                                break;
                        }
                        if (dir!='x' && dir!='z') {
                            move(N,dir,A);
                            spawn(N,A);
                        }
                    }
                }

                if (status==MENU) {
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

                    main_menu_display(e, &status, &A, N);
                }
            }
        }
    }

        /*
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