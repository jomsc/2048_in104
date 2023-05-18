#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
Uint32 elapsedTime = 0;
Uint32 musicStartTime = 0;
int splash = 0;
int splashLen = 12;
int musicMultiplier = -1;


Mix_Chunk* start = NULL;
Mix_Chunk* loose = NULL;
Mix_Chunk* onCombine = NULL;
Mix_Chunk* victoryChunk = NULL;

int max_i(int a, int b) {
    if (a>b) { return a; }
    else { return b; }
}

enum status { MENU, CLASSIC, MUSIC, DIFFICULTY, NDIM, NDIM_SELECT, VICTORY, DEFEAT, QUIT };

char MOTD[12][40] = {"By Clara and Joseph!","Music by @48Ocean","SONO approved!","merci jaja",
                     "Seen on TV!","French Touch = good music","It's real!","Bugs included",
                     "From ENSTA with love","...!","Works in Australia!","STARDUST?! Coming soon..."};

int test(int x, int y) { return (x==y); }
int f(int x, int y) { return x+y; }
void combine(int i, char dir, int N, int* A, int* score, int multiplier)
{
    if (dir=='r') {
        int n=0;
        for (int j=0;j<N;j++) { if (A[i*N+j]!=-1) { n++; } }
        int* pos=malloc(sizeof(int)*n);
        int j3=0;
        for (int j2=N-1;j2>=0;j2--) { if (A[i*N+j2]!=-1) { pos[j3]=j2; j3++; } }
        
        for (int k=0;k<n-1;k++) { 
            if (test(A[i*N+pos[k]],A[i*N+pos[k+1]])) {
                *score += f(A[i*N+pos[k]],A[i*N+pos[k+1]])*(pow(2,multiplier+1));
                A[i*N+pos[k]] = f(A[i*N+pos[k]],A[i*N+pos[k+1]]);
                A[i*N+pos[k+1]] = -1;
                k++;
                //Mix_PlayChannel( -1, onCombine, 0 );
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
                *score += f(A[i*N+pos[k]],A[i*N+pos[k+1]])*(pow(2,multiplier+1));
                A[i*N+pos[k]] = f(A[i*N+pos[k]],A[i*N+pos[k+1]]);
                A[i*N+pos[k+1]] = -1;
                k++;
                //Mix_PlayChannel( -1, onCombine, 0 );
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
                *score += f(A[pos[k]*N+i],A[pos[k+1]*N+i])*(pow(2,multiplier+1));
                A[pos[k]*N+i] = f(A[pos[k]*N+i],A[pos[k+1]*N+i]);
                A[pos[k+1]*N+i] = -1;
                k++;
                //Mix_PlayChannel( -1, onCombine, 0 );
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
                *score += f(A[pos[k]*N+i],A[pos[k+1]*N+i])*(pow(2,multiplier+1));
                A[pos[k]*N+i] = f(A[pos[k]*N+i],A[pos[k+1]*N+i]);
                A[pos[k+1]*N+i] = -1;
                k++;
                //Mix_PlayChannel( -1, onCombine, 0 );
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
void move(int N, char dir, int* A, int* score, int multiplier) {
    for (int i=0;i<N;i++) {
        combine(i,dir,N,A,score,multiplier);
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
bool isVictory (int* A, int N) {
    bool victory = false;
    for (int i=0;i<N*N;i++) {
        if (A[i] == 2048) { victory = true; }
    }

    return victory;
}
bool isDefeat (int* A, int N) {
    bool defeat = true;
    for (int i=0;i<N-1;i++) {
        for (int j=0;j<N-1;j++) {
            if (A[N*i+j]==-1 || (A[N*i+j]==A[N*i+j+1]) || (A[N*(i+1)+j]==A[N*i+j])) {
                defeat = false;
            }
        }
    }

    return defeat;
}
bool isInside(int x, int y, int w, int h, int a, int b) {
    return ((x<=(a+w) && x>=a) && (y<=(b+h) && y>= b));
}

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

Mix_Music* menuMusic = NULL;
Mix_Music* musicD1 = NULL;
Mix_Music* musicD2 = NULL;


SDL_Texture* menuVideo = NULL;
SDL_Texture* menuButtons = NULL;
SDL_Texture* resetButton = NULL;
SDL_Texture* menuTitle = NULL;
SDL_Texture* victoryTexture = NULL;
SDL_Texture* defeatTexture = NULL;
SDL_Texture* multiplierTexture = NULL;

SDL_Texture* classicBackgroundTexture = NULL;
SDL_Texture* numbersTexture = NULL;
TTF_Font* scoreFont = NULL;
TTF_Font* splashFont = NULL;
SDL_Texture* scoreTexture = NULL;
SDL_Texture* scoreBGTexture = NULL;
SDL_Texture* splashTexture = NULL;
int wSplash;
int hSplash;
int wText;
int hText;

SDL_Texture* diff_buttons = NULL;
SDL_Texture* barreTexture = NULL;
SDL_Texture* arrowUp = NULL;
SDL_Texture* arrowDown = NULL;
SDL_Texture* dimTexture = NULL;
SDL_Texture* goTexture = NULL;

bool isInitial[3] = {true, true, true};
bool vicIgnore[3] = {false, false, false};


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

bool loadFromRenderedText (char* textureText, SDL_Color textColor, SDL_Texture** textTexture, TTF_Font* textFont) {
    SDL_Surface* textSurface = TTF_RenderText_Solid( textFont, textureText, textColor );
    if( textSurface == NULL ) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        *textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( *textTexture == NULL ) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else {
            //Get image dimensions
            wText = textSurface->w;
            hText = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return *textTexture != NULL;
}
bool loadText (char* text, SDL_Texture** textTexture, TTF_Font** textFont, char* pathToFont, int size, SDL_Color textColor) {
    bool success = true;
    *textFont = TTF_OpenFont(pathToFont, size);
    if (*textFont == NULL) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else  {
        if(!loadFromRenderedText(text,textColor,textTexture,*textFont))
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }

    return success;
}


bool textureInit() {
    Uint32 startTime = SDL_GetTicks();

    if (!loadMedia("../../assets/textures/rect_mega.png", &numbersTexture)) {
        printf("Failed to load numbers texture !");
    }

    if (!loadMedia("../../assets/textures/background.png", &classicBackgroundTexture)) {
        printf("Failed to load game background!");
    }

    if (!loadMedia("../../assets/textures/reset.png", &resetButton)) {
        printf("Failed to load reset button!");
    }

    if (!loadMedia("../../assets/textures/classic/score.png", &scoreBGTexture)) {
        printf("Failed to load score background!");
    }

    if (!loadMedia("../../assets/textures/buttons.png",&menuButtons)) {
        printf("Failed to load menu buttons!");
    }


    if (!loadMedia("../../assets/videos/menu/out.jpeg",&menuVideo)) {
        printf("coup dur");
    }
    printf("loaded video");
    if (!loadMedia("../../assets/textures/title.png", &menuTitle)) {
        printf("Failed to load title texture!");
    }

    if (!loadMedia("../../assets/textures/victory.png", &victoryTexture)) {
        printf("Failed to load victory texture!");
    }

    if (!loadMedia("../../assets/textures/defeat.png", &defeatTexture)) {
        printf("Failed to load defeat texture!");
    }

    if (!loadMedia("../../assets/textures/diff_select.png", &diff_buttons)) {
        printf("Failed to load difficulty buttons texture!");
    }

    if (!loadMedia("../../assets/textures/music/barre.png", &barreTexture)) {
        printf("Failed to load barre texture!");
    }

    if (!loadMedia("../../assets/textures/multiplier.png", &multiplierTexture)) {
        printf("Failed to load multiplier texture!");
    }

    if (!loadMedia("../../assets/textures/arrowup.png", &arrowUp)) {
        printf("Failed to load arrowUp texture!");
    }

    if (!loadMedia("../../assets/textures/arrowdown.png", &arrowDown)) {
        printf("Failed to load arrowDown texture!");
    }

    if (!loadMedia("../../assets/textures/go.png", &goTexture)) {
        printf("Failed to load go texture!");
    }

    Uint32 stopTime = SDL_GetTicks();
    printf("Time to load : %f s\n",(stopTime-startTime)/1000.0);

    return true;
}
bool musicInit() {
    bool success = true;

    menuMusic = Mix_LoadMUS("../../assets/audio/music/1.wav");
    if (menuMusic == NULL) {
        printf( "Failed to load menu music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    musicD1 = Mix_LoadMUS("../../assets/audio/music/2.wav");
    if (musicD1 == NULL) {
        printf( "Failed to load D1 music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    musicD2 = Mix_LoadMUS("../../assets/audio/music/3.wav");
    if (musicD2 == NULL) {
        printf( "Failed to load D2 music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    start = Mix_LoadWAV("../../assets/audio/chunk/start.wav");
    if (start == NULL) {
        printf("Failed to load start chunk! SDL_Mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    loose = Mix_LoadWAV("../../assets/audio/chunk/loose.wav");
    if (loose == NULL) {
        printf("Failed to load loose chunk! SDL_Mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    onCombine = Mix_LoadWAV("../../assets/audio/chunk/onCombine.wav");
    if (onCombine == NULL) {
        printf("Failed to load onCombine chunk! SDL_Mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    victoryChunk = Mix_LoadWAV("../../assets/audio/chunk/victory.wav");
    if (victoryChunk == NULL) {
        printf("Failed to load victory chunk! SDL_Mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}
bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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
        gWindow = SDL_CreateWindow( "Super 2048!", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

        //SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);
        //SDL_GetWindowSize(gWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);

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

                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error : %s\n", TTF_GetError());
                    success = false;
                }

                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }

        textureInit();
        musicInit();

        SDL_Surface* icon = IMG_Load("../../assets/textures/icon.png");
        SDL_SetWindowIcon(gWindow, icon);
    }

    return success;
}
void wclose()
{
    SDL_DestroyTexture(gTexture);
    SDL_DestroyTexture(menuButtons);
    SDL_DestroyTexture(resetButton);
    SDL_DestroyTexture(menuVideo);
    SDL_DestroyTexture(menuTitle);
    SDL_DestroyTexture(numbersTexture);
    SDL_DestroyTexture(classicBackgroundTexture);
    SDL_DestroyTexture(victoryTexture);
    SDL_DestroyTexture(diff_buttons);

    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(musicD1);
    Mix_FreeMusic(musicD2);
    menuMusic = NULL;
    musicD1 = NULL;
    musicD2 = NULL;

    Mix_FreeChunk(start);
    Mix_FreeChunk(loose);
    Mix_FreeChunk(onCombine);
    Mix_FreeChunk(victoryChunk);
    start = NULL;
    loose = NULL;
    onCombine = NULL;
    victoryChunk = NULL;

    SDL_DestroyRenderer( gRenderer);
    SDL_DestroyWindow( gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    gTexture = NULL;

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
}

void classic_display(int* A, int N, int score) {
    elapsedTime=SDL_GetTicks();
    isInitial[0] = false;
    int margin = 20;
    int rect_size = 100;
    int offsetX = SCREEN_WIDTH/2-(N*rect_size+(N+1)*margin)/2;
    int offsetY = SCREEN_HEIGHT/2-(N*rect_size+(N+1)*margin)/2;
    int* posx = malloc(sizeof(int)*N*N);
    int* posy = malloc(sizeof(int)*N*N);
    int* srcX = malloc(sizeof(int)*16);
    int* srcY = malloc(sizeof(int)*16);
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            posy[N*i+j] = offsetY+margin+(rect_size+margin)*i;
            posx[N*i+j] = offsetX+margin+(rect_size+margin)*j;
        }
    }

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            srcX[4*i+j] = 512*j;
            srcY[4*i+j] = 512*i;
        }
    }

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);

    SDL_Rect backgroundRect = { offsetX, offsetY, N*rect_size+(N+1)*margin, N*rect_size+(N+1)*margin};
    SDL_RenderCopy(gRenderer, classicBackgroundTexture, NULL, &backgroundRect);

    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            int number = A[N*i+j];

            if (number == -1) {
                SDL_Rect srcRect = {srcX[15], srcY[15], 512, 512};
                SDL_Rect textureRect = {posx[N*i+j], posy[N*i+j], rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numbersTexture, &srcRect, &textureRect);
            }
            else {
                int a = (int) (log(number)/log(2)-1);
                SDL_Rect srcRect = {srcX[a], srcY[a], 512, 512};
                SDL_Rect textureRect = {posx[N * i + j], posy[N * i + j], rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numbersTexture, &srcRect, &textureRect);
            }
        }
    }

    SDL_Rect scoreHeaderRect = { offsetX+(N+2)*margin+N*rect_size, offsetY, 200, 148 };
    SDL_RenderCopy(gRenderer, scoreBGTexture, NULL, &scoreHeaderRect);

    SDL_Color color = {255, 185, 167};
    char score_c[20];
    strcpy(score_c, "");
    sprintf(score_c, "%d", score);
    loadText(score_c, &scoreTexture, &scoreFont,
             "../../assets/fonts/ITCAvantGardeStd-Bold.ttf",
             50, color);
    SDL_QueryTexture(scoreTexture, NULL, NULL, &wText, &hText);

    SDL_Rect scoreRect = { offsetX+(N+2)*margin+N*rect_size+20, offsetY+2*margin+50, wText, hText };
    SDL_RenderCopy(gRenderer, scoreTexture, NULL, &scoreRect);


    SDL_RenderPresent(gRenderer);

    Uint32 timeCount = SDL_GetTicks();
    printf("Time to render : %d ms\n",timeCount-elapsedTime);

    free(posx);
    free(posy);
    free(videoTextureX);
    free(videoTextureY);

    TTF_CloseFont(scoreFont);
}
void menu_display(SDL_Event e, enum status *status, int* A[], int* B[], int* C[], int N,
        int* score, int x, int y, bool clicked, int* musicScore, int* ndimscore, int dim_grid) {
    elapsedTime = SDL_GetTicks();
    int nb_buttons=10;
    int initialMargin = 250;
    int margin = 20;
    int wButton = 256;
    int hButton = 64;
    int* posx = malloc(sizeof(int)*nb_buttons);
    int* posy = malloc(sizeof(int)*nb_buttons);
    int* textureX = malloc(sizeof(int)*nb_buttons);
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<nb_buttons;i++) {
        posy[i] = initialMargin+(hButton+margin)*i;
        posx[i] = SCREEN_WIDTH/2-wButton/2;
        textureX[i] = 1024*(i%5);
    }

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    SDL_RenderClear(gRenderer);

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);


    // TITLE
    SDL_Rect textureRect = {SCREEN_WIDTH/2-2*wButton, 0, 4*wButton, 4*hButton};
    SDL_RenderCopy(gRenderer, menuTitle, NULL, &textureRect);

    // load buttons
    for (int i=0;i<(nb_buttons/2);i++) {
        if (i!=3) {
            SDL_Rect textureRect = {posx[i], posy[i], wButton, hButton};
            if (isInside(x,y,wButton, hButton,posx[i], posy[i])) {
                SDL_Rect srcRect = {textureX[i], 256, 1024, 256};
                SDL_RenderCopy(gRenderer, menuButtons, &srcRect, &textureRect);
            } else {
                SDL_Rect srcRect = {textureX[i], 0, 1024, 256};
                SDL_RenderCopy(gRenderer, menuButtons, &srcRect, &textureRect);
            }

            if (i==0) {
                if (!isInitial[i]) {
                    SDL_Rect resetRect = { posx[i]+wButton+margin, posy[i], hButton, hButton};
                    SDL_RenderCopy(gRenderer, resetButton, NULL, &resetRect);
                }
            }

            if (i==1) {
                if (!isInitial[i]) {
                    SDL_Rect resetRect = { posx[i]+wButton+margin, posy[i], hButton, hButton};
                    SDL_RenderCopy(gRenderer, resetButton, NULL, &resetRect);
                }
            }

            if (i==2) {
                if (!isInitial[2]) {
                    SDL_Rect resetRect = { posx[i]+wButton+margin, posy[i], hButton, hButton};
                    SDL_RenderCopy(gRenderer, resetButton, NULL, &resetRect);
                }
            }
        }
    }

    // MOTD
    SDL_Color splashColor = { 255, 255, 0};
    if (!loadText(MOTD[splash],&splashTexture,&splashFont,
                  "../../assets/fonts/minecraft_font.ttf",
                  60,splashColor)) {
        printf("failed to load MOTD");
    }
    else {
        double size = (sin((elapsedTime%2000)/314.0)+10)*0.1;
        SDL_QueryTexture(splashTexture, NULL, NULL, &wSplash, &hSplash);
        wSplash = (int)(wSplash*size/3);
        hSplash = (int)(hSplash*size/3);
        SDL_Rect splashRect = { SCREEN_WIDTH/2+wButton/2, (int)(2.3*hButton), wSplash, hSplash};
        if (splash==10) {
            SDL_RenderCopyEx(gRenderer, splashTexture, NULL,
                             &splashRect,-20,NULL,
                             SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        }
        else {
            SDL_RenderCopyEx(gRenderer, splashTexture, NULL,
                             &splashRect,-20,NULL,SDL_FLIP_NONE);
        }
    }

    if (isInside(x,y,wButton,hButton,posx[0],posy[0]) && clicked) {
        *status = CLASSIC;
        Mix_PlayChannel( 3, start, 0 );
    }
    if (isInside(x,y,hButton,hButton,posx[0]+wButton+margin,posy[0]) && !isInitial[0] && clicked) {
        isInitial[0] = true;
        *score = 0;
        for (int k=0;k<(N*N);k++) {
            (*A)[k]=-1;
            printf("%d", k);
        }
        spawn(N, *A);
        vicIgnore[0] = false;
    }
    if (isInside(x,y,hButton,hButton,posx[1]+wButton+margin,posy[1]) && !isInitial[1] && clicked) {
        isInitial[1] = true;
        *musicScore = 0;
        for (int k=0;k<(N*N);k++) {
            (*B)[k]=-1;
            printf("%d", k);
        }
        spawn(N, *B);
        vicIgnore[1] = false;
        musicMultiplier=-1;
    }
    if (isInside(x,y,hButton,hButton,posx[2]+wButton+margin,posy[2]) && !isInitial[2] && clicked) {
        isInitial[2] = true;
        *ndimscore = 0;
        free((*C));
        (*C)=NULL;
        vicIgnore[2] = false;
        musicMultiplier=-1;
    }
    if (isInside(x,y,wButton,hButton,posx[1],posy[1]) && clicked) {
        *status = DIFFICULTY;
        Mix_PlayChannel( -1, start, 0 );
    }
    if (isInside(x,y,wButton,hButton,posx[2],posy[2]) && clicked) {
        *status = NDIM_SELECT;
        Mix_PlayChannel( -1, start, 0 );
    }
    if (isInside(x,y,wButton,hButton,posx[4],posy[4]) && clicked) { *status = QUIT; }

    SDL_RenderPresent(gRenderer);


    free(posx);
    free(posy);
    free(textureX);
    free(videoTextureX);
    free(videoTextureY);

    SDL_DestroyTexture(gTexture);
    SDL_DestroyTexture(splashTexture);
    TTF_CloseFont(splashFont);

    Uint32 timeCount = SDL_GetTicks();
    printf("Time to render : %d ms\n",timeCount-elapsedTime);
}
void music_display(int difficulty, int* A, int N, int score, bool *pressed, int *multiplier) {
    elapsedTime = SDL_GetTicks();

    isInitial[1] = false;
    int margin = 20;
    int rect_size = 100;
    int offsetX = SCREEN_WIDTH/2-(N*rect_size+(N+1)*margin)/2;
    int offsetY = 50;
    int* posx = malloc(sizeof(int)*N*N);
    int* posy = malloc(sizeof(int)*N*N);
    int* srcX = malloc(sizeof(int)*16);
    int* srcY = malloc(sizeof(int)*16);
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            posy[N*i+j] = offsetY+margin+(rect_size+margin)*i;
            posx[N*i+j] = offsetX+margin+(rect_size+margin)*j;
        }
    }

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            srcX[N*i+j] = 512*j;
            srcY[N*i+j] = 512*i;
        }
    }

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);

    SDL_Rect barreDst = {SCREEN_WIDTH/2-256, 600, 512, 64};
    SDL_RenderCopy(gRenderer, barreTexture, NULL, &barreDst);

    int bpm = 100;
    if (difficulty == 1) {
        bpm = 106;
    }
    if (difficulty == 2) {
        bpm = 120;
    }
    double speed = bpm/120.0;
    Uint32 timeTime = SDL_GetTicks();
    int cursorX = SCREEN_WIDTH/2-256+487*(((int)(2*speed*(timeTime-musicStartTime+600)))%1000)/1000.0;
    printf("cursorX : %d\n", cursorX);

    SDL_Rect cursorRect = { cursorX, 582, 25, 100 };
    if (cursorX > SCREEN_WIDTH/2-256+375) {
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0xCF, 0x00, 0xFF );
    } else {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }

    if (*pressed) {
        if (cursorX<(215+SCREEN_WIDTH/2-256) && cursorX ) {
            if (*multiplier > 0) { *multiplier = *multiplier-2; }
            else { *multiplier = -1; }
        }
        if (cursorX<(300+SCREEN_WIDTH/2-256)) {
            if (*multiplier > -1) { (*multiplier)--; }
        }
        else {
            if ((*multiplier)<7) { (*multiplier)++; }
            else { *multiplier=7; }
        }
    }
    *pressed = false;

    SDL_RenderFillRect( gRenderer, &cursorRect );

    SDL_Rect backgroundRect = { offsetX, offsetY, N*rect_size+(N+1)*margin, N*rect_size+(N+1)*margin};
    SDL_RenderCopy(gRenderer, classicBackgroundTexture, NULL, &backgroundRect);


    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            int number = A[N*i+j];

            if (number == -1) {
                SDL_Rect srcRect = {srcX[15], srcY[15], 512, 512};
                SDL_Rect textureRect = {posx[N * i + j], posy[N * i + j], rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numbersTexture, &srcRect, &textureRect);
            }
            else {
                int a = (int) (log(number)/log(2)-1);
                SDL_Rect srcRect = {srcX[a], srcY[a], 512, 512};
                SDL_Rect textureRect = {posx[N * i + j], posy[N * i + j], rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numbersTexture, &srcRect, &textureRect);
            }
        }
    }

    SDL_Rect scoreHeaderRect = { offsetX+(N+2)*margin+N*rect_size, offsetY, 200, 148 };
    SDL_RenderCopy(gRenderer, scoreBGTexture, NULL, &scoreHeaderRect);

    SDL_Color color = {255, 185, 167};
    char score_c[20];
    strcpy(score_c, "");
    sprintf(score_c, "%d", score);
    loadText(score_c, &scoreTexture, &scoreFont,
             "../../assets/fonts/ITCAvantGardeStd-Bold.ttf",
             50, color);
    SDL_QueryTexture(scoreTexture, NULL, NULL, &wText, &hText);

    SDL_Rect scoreRect = { offsetX+(N+2)*margin+N*rect_size+20, offsetY+2*margin+50, wText, hText };
    SDL_RenderCopy(gRenderer, scoreTexture, NULL, &scoreRect);

    double size = (sin(((int)(4*speed*elapsedTime)%2000)/314.0)+10)*0.1;
    if (*multiplier != -1) {
        SDL_Rect multiSrc = { (*multiplier)*256, 0, 256, 256};
        SDL_Rect multiDst = { 150+ offsetX+(N+2)*margin+N*rect_size, offsetY+2*margin+70, size*120, size*100};
        SDL_RenderCopyEx(gRenderer, multiplierTexture, &multiSrc,
                         &multiDst,20,NULL,SDL_FLIP_NONE);
    }
    SDL_RenderPresent(gRenderer);

    Uint32 timeCount = SDL_GetTicks();
    printf("Time to render : %d ms\n",timeCount-elapsedTime);

    free(posx);
    free(posy);
    free(videoTextureX);
    free(videoTextureY);
    TTF_CloseFont(scoreFont);
}
void victory_display(int x, int y, bool clicked, enum status *status, int previous) {
    elapsedTime=SDL_GetTicks();
    Mix_PauseMusic();
    Mix_PlayChannel( -1, victoryChunk, 0 );
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);


    SDL_Rect vicRect = {SCREEN_WIDTH/2 - 960/2, 200, 960, 240};
    SDL_RenderCopy(gRenderer, victoryTexture, NULL, &vicRect);

    if (!isInside(x, y, 256, 64, SCREEN_WIDTH/2 - 300, 440)) {
        SDL_Rect continueDst = {SCREEN_WIDTH/2 - 300, 440, 256, 64};
        SDL_Rect continueSrc = { 5*1024, 0, 1024, 256};
        SDL_RenderCopy(gRenderer, menuButtons, &continueSrc, &continueDst);
    } else {
        SDL_Rect continueDst = {SCREEN_WIDTH/2 - 300, 440, 256, 64};
        SDL_Rect continueSrc = { 5*1024, 256, 1024, 256};
        SDL_RenderCopy(gRenderer, menuButtons, &continueSrc, &continueDst);

        if (clicked) {
            if (previous==1) {
                *status = CLASSIC;
                vicIgnore[0] = true;
            }
            if (previous==2) {
                *status = MUSIC;
                vicIgnore[1] = true;
            }
            if (previous==3) {
                *status = NDIM;
                vicIgnore[2] = true;
            }
            Mix_ResumeMusic();
        }
    }

    if (!isInside(x, y, 256, 64, SCREEN_WIDTH/2 + 44, 440)) {
        SDL_Rect exitDst = {SCREEN_WIDTH/2 + 44, 440, 256, 64};
        SDL_Rect exitSrc = { 6*1024, 0, 1024, 256};
        SDL_RenderCopy(gRenderer, menuButtons, &exitSrc, &exitDst);
    } else {
        SDL_Rect exitDst = {SCREEN_WIDTH/2 + 44, 440, 256, 64};
        SDL_Rect exitSrc = { 6*1024, 256, 1024, 256};
        SDL_RenderCopy(gRenderer, menuButtons, &exitSrc, &exitDst);

        if (clicked) {
            *status = MENU;
            Mix_ResumeMusic();
        }
    }



    SDL_RenderPresent(gRenderer);

    Uint32 timeCount = SDL_GetTicks();
    printf("Time to render : %d ms\n",timeCount-elapsedTime);

    free(videoTextureX);
    free(videoTextureY);
}
void defeat_display(int x, int y, bool clicked, enum status *status, int* A[], int* B[],
        int* C[], int N, int* score, int previous) {
    elapsedTime=SDL_GetTicks();
    Mix_PauseMusic();
    Mix_PlayChannel( -1, loose, 0 );
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);

    SDL_Rect defRect = {SCREEN_WIDTH/2 - 960/2, 200, 960, 240};
    SDL_RenderCopy(gRenderer, defeatTexture, NULL, &defRect);

    if (!isInside(x, y, 256, 64, SCREEN_WIDTH/2 - 128, 440)) {
        SDL_Rect exitDst = {SCREEN_WIDTH/2 - 128, 440, 256, 64};
        SDL_Rect exitSrc = { 6*1024, 0, 1024, 256};
        SDL_RenderCopy(gRenderer, menuButtons, &exitSrc, &exitDst);
    } else {
        SDL_Rect exitDst = {SCREEN_WIDTH/2 - 128, 440, 256, 64};
        SDL_Rect exitSrc = { 6*1024, 256, 1024, 256};
        SDL_RenderCopy(gRenderer, menuButtons, &exitSrc, &exitDst);

        if (clicked) {
            if (previous==1) {
                *status = MENU;
                Mix_ResumeMusic();
                for (int i=0;i<N*N;i++) { (*A)[i]=-1; }
                *score = 0;
                spawn(N, *A);
                isInitial[0] = true;
            }

            if (previous==2) {
                *status = MENU;
                Mix_ResumeMusic();
                for (int i=0;i<N*N;i++) { (*B)[i]=-1; }
                *score = 0;
                spawn(N, *B);
                isInitial[1] = true;
            }

            if (previous==3) {
                *status = MENU;
                Mix_ResumeMusic();
                for (int i=0;i<N*N;i++) { (*C)[i]=-1; }
                *score = 0;
                spawn(N, *C);
                isInitial[2] = true;
            }
        }
    }

    SDL_RenderPresent(gRenderer);

    Uint32 timeCount = SDL_GetTicks();
    printf("Time to render : %d ms\n",timeCount-elapsedTime);

    free(videoTextureX);
    free(videoTextureY);
}
void diff_display(int x, int y, bool clicked, enum status *status, int *difficulty) {
    elapsedTime=SDL_GetTicks();
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);

    if (!isInside(x, y, 256, 64, SCREEN_WIDTH/2 - 300, 380)) {
        SDL_Rect midDst = {SCREEN_WIDTH/2 - 300, 380, 256, 64};
        SDL_Rect midSrc = { 0, 0, 1024, 256};
        SDL_RenderCopy(gRenderer, diff_buttons, &midSrc, &midDst);
    } else {
        SDL_Rect midDst = {SCREEN_WIDTH/2 - 300, 380, 256, 64};
        SDL_Rect midSrc = { 0, 256, 1024, 256};
        SDL_RenderCopy(gRenderer, diff_buttons, &midSrc, &midDst);

        if (clicked) {
            *status = MUSIC;
            *difficulty = 1;
            Mix_HaltMusic();
            Mix_PlayMusic(musicD1, -1);
            musicStartTime = SDL_GetTicks();
        }
    }

    if (!isInside(x, y, 256, 64, SCREEN_WIDTH/2 + 44, 380)) {
        SDL_Rect hardDst = {SCREEN_WIDTH/2 + 44, 380, 256, 64};
        SDL_Rect hardSrc = { 1024, 0, 1024, 256};
        SDL_RenderCopy(gRenderer, diff_buttons, &hardSrc, &hardDst);
    } else {
        SDL_Rect hardDst = {SCREEN_WIDTH/2 + 44, 380, 256, 64};
        SDL_Rect hardSrc = { 1024, 256, 1024, 256};
        SDL_RenderCopy(gRenderer, diff_buttons, &hardSrc, &hardDst);

        if (clicked) {
            *status = MUSIC;
            *difficulty = 2;
            Mix_HaltMusic();
            Mix_PlayMusic(musicD2, -1);
            musicStartTime = SDL_GetTicks();
        }
    }


    SDL_RenderPresent(gRenderer);

    free(videoTextureX);
    free(videoTextureY);
}
void ndim_select_display(int *dim_grid, int x, int y, bool clicked, enum status* status, int *A[]) {
    if (!isInitial[2]) {
        *status=NDIM;
    }
    else {
        elapsedTime=SDL_GetTicks();
        int* videoTextureX = malloc(sizeof(int)*107);
        int* videoTextureY = malloc(sizeof(int)*107);

        for (int i=0;i<13;i++) {
            for (int j=0;j<8;j++) {
                videoTextureY[8*i+j]=1080*i;
                videoTextureX[8*i+j]=1920*j;
            }
        }

        for (int j=0;j<3;j++) {
            videoTextureY[8*13+j]=1080*13;
            videoTextureX[8*13+j]=1920*j;
        }

        SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
        SDL_RenderClear(gRenderer);

        int bgFrameCounter;
        elapsedTime = SDL_GetTicks();
        bgFrameCounter = (int)((elapsedTime%3566)/34);
        SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
        SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);


        SDL_Rect upRect = {SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT/2 - 110, 150, 100};
        SDL_RenderCopy(gRenderer, arrowUp, NULL, &upRect);

        SDL_Rect downRect = {SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT/2 +10, 150, 100};
        SDL_RenderCopy(gRenderer, arrowDown, NULL, &downRect);

        SDL_Rect bgRect = {SCREEN_WIDTH/2 - 140, SCREEN_HEIGHT/2 - 110, 220, 220};
        SDL_RenderCopy(gRenderer, classicBackgroundTexture, NULL, &bgRect);

        SDL_Color color = { 255, 255, 255};
        char dim_c[20];
        strcpy(dim_c, "");
        sprintf(dim_c, "%d", *dim_grid);
        loadText(dim_c, &dimTexture, &scoreFont,
                 "../../assets/fonts/ITCAvantGardeStd-Bold.ttf",
                 200, color);
        SDL_QueryTexture(dimTexture, NULL, NULL, &wText, &hText);

        SDL_Rect dimRect = { 20+(int)((SCREEN_WIDTH/2-140)+(max_i(wText,hText)-wText)/2),
                             35+(int)((SCREEN_HEIGHT/2-110)+(max_i(wText,hText)-hText)/2),
                             (int)(wText*180/max_i(wText, hText)),
                             (int)(hText*180/max_i(wText, hText)) };
        SDL_RenderCopy(gRenderer, dimTexture, NULL, &dimRect);

        if (isInside(x,y,150,100,SCREEN_WIDTH/2 + 100,SCREEN_HEIGHT/2 - 110) && clicked && *dim_grid<10) {
            *dim_grid += 1;
        }

        if (isInside(x,y,150,100,SCREEN_WIDTH/2 + 100,SCREEN_HEIGHT/2 + 10) && clicked && *dim_grid>2) {
            *dim_grid -= 1;
        }

        if (!isInside(x,y,256,64,SCREEN_WIDTH/2-128,SCREEN_HEIGHT/2+140)) {
            SDL_Rect goSrc = {0, 0, 1024, 256};
            SDL_Rect goDst = {SCREEN_WIDTH/2-128, SCREEN_HEIGHT/2+140, 256, 64};
            SDL_RenderCopy(gRenderer,goTexture,&goSrc,&goDst);
        } else {
            SDL_Rect goSrc = {0, 256, 1024, 256};
            SDL_Rect goDst = {SCREEN_WIDTH/2-128, SCREEN_HEIGHT/2+140, 256, 64};
            SDL_RenderCopy(gRenderer,goTexture,&goSrc,&goDst);

            if (clicked) {
                *status = NDIM;
                if (*A==NULL) {
                    free((*A));
                    (*A)=malloc(sizeof(int)*(*dim_grid)*(*dim_grid));
                    for (int i=0;i<(*dim_grid)*(*dim_grid);i++) {
                        (*A)[i] = -1;
                    }
                    spawn(*dim_grid, *A);
                }
            }
        }

        SDL_RenderPresent(gRenderer);

        Uint32 timeCount = SDL_GetTicks();
        printf("Time to render : %d ms\n",timeCount-elapsedTime);
        TTF_CloseFont(scoreFont);

        free(videoTextureX);
        free(videoTextureY);
    }
}
void ndim_display(int* A, int N, int score) {
    isInitial[2] = false;
    elapsedTime=SDL_GetTicks();
    int width = 600;
    int margin = 50/N;
    int rect_size=(int)((width-(N-1)*margin-2*20)/N);
    SDL_SetRenderDrawColor( gRenderer, 0xFE, 0xFE, 0xE6, 0xFF );
    SDL_RenderClear(gRenderer);
    int offsetX=SCREEN_WIDTH/2-width/2;
    int offsetY=SCREEN_HEIGHT/2-width/2;

    int* srcX = malloc(sizeof(int)*16);
    int* srcY = malloc(sizeof(int)*16);

    elapsedTime=SDL_GetTicks();
    int* videoTextureX = malloc(sizeof(int)*107);
    int* videoTextureY = malloc(sizeof(int)*107);

    for (int i=0;i<13;i++) {
        for (int j=0;j<8;j++) {
            videoTextureY[8*i+j]=1080*i;
            videoTextureX[8*i+j]=1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        videoTextureY[8*13+j]=1080*13;
        videoTextureX[8*13+j]=1920*j;
    }

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            srcX[4*i+j] = 512*j;
            srcY[4*i+j] = 512*i;
        }
    }

    int bgFrameCounter;
    elapsedTime = SDL_GetTicks();
    bgFrameCounter = (int)((elapsedTime%3566)/34);
    SDL_Rect vsrcRect = {videoTextureX[bgFrameCounter+1],videoTextureY[bgFrameCounter+1],1920,1080};
    SDL_RenderCopy(gRenderer, menuVideo, &vsrcRect, NULL);

    SDL_Rect backgroundRect = { offsetX, offsetY, width, width };
    SDL_RenderCopy(gRenderer, classicBackgroundTexture, NULL, &backgroundRect);


    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            int number = A[N*i+j];
            printf("%d   |",number);

            if (number == -1) {
                SDL_Rect srcRect = {srcX[15], srcY[15], 512, 512};
                SDL_Rect textureRect = {offsetX+j*rect_size+j*margin+20, offsetY+i*rect_size+i*margin+20,
                                        rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numbersTexture, &srcRect, &textureRect);
            }
            else {
                int a = (int)(log(number)/log(2)-1);
                SDL_Rect srcRect = {srcX[a], srcY[a], 512, 512};
                SDL_Rect textureRect = {offsetX+j*rect_size+j*margin+20, offsetY+i*rect_size+i*margin+20,
                                        rect_size, rect_size};
                SDL_RenderCopy(gRenderer, numbersTexture, &srcRect, &textureRect);
            }
        }
        printf("\n");
    }

    SDL_Rect scoreHeaderRect = { offsetX+(N-1)*margin+N*rect_size+2*20+30, offsetY, 200, 148 };
    SDL_RenderCopy(gRenderer, scoreBGTexture, NULL, &scoreHeaderRect);

    SDL_Color color = {255, 185, 167};
    char score_c[20];
    strcpy(score_c, "");
    sprintf(score_c, "%d", score);
    loadText(score_c, &scoreTexture, &scoreFont,
             "../../assets/fonts/ITCAvantGardeStd-Bold.ttf",
             50, color);
    SDL_QueryTexture(scoreTexture, NULL, NULL, &wText, &hText);
    SDL_Rect scoreRect = { offsetX+(N-1)*margin+N*rect_size+2*20+40, offsetY+90, wText, hText };
    SDL_RenderCopy(gRenderer, scoreTexture, NULL, &scoreRect);



    SDL_RenderPresent(gRenderer);
    Uint32 timeCount = SDL_GetTicks();
    printf("Time to render : %d ms\n",timeCount-elapsedTime);

    free(srcX);
    free(srcY);
    free(videoTextureX);
    free(videoTextureY);
}


int main()
{
    enum status status = MENU;
    int dim_grid = 4;
    srand(time(NULL));
    int N=4;
    int* gridClassic=malloc(sizeof(int)*N*N);
    for (int i=0;i<N*N;i++) { gridClassic[i]=-1; }

    int* gridMusic=malloc(sizeof(int)*N*N);
    for (int i=0;i<N*N;i++) { gridMusic[i]=-1; }

    int* gridNdim = NULL;

    char dir;

    int score = 0;
    int musicScore = 0;
    int ndimscore = 0;

    int difficulty = 1;
    int previous=1;

    spawn(N,gridClassic);
    spawn(N,gridMusic);


    int x, y;
    bool clicked = false;
    bool pressed = false;

    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        bool quit = false;
        SDL_Event e;
        Mix_PlayMusic( menuMusic, -1 );
        while( !quit )
        {
            elapsedTime = SDL_GetTicks();
            clicked = false;
            pressed = false;

            while( SDL_PollEvent( &e ) ) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN) {
                    pressed = true;
                    switch (e.key.keysym.sym) {
                        case SDLK_z:
                            dir = 'u';
                            break;
                        case SDLK_s:
                            dir = 'd';
                            break;
                        case SDLK_q:
                            dir = 'l';
                            break;
                        case SDLK_d:
                            dir = 'r';
                            break;
                        case SDLK_ESCAPE:
                            if (status != MENU) {
                                splash = rand()%splashLen;
                                if (status != CLASSIC && status != NDIM_SELECT && status != NDIM) {
                                    Mix_HaltMusic();
                                    Mix_PlayMusic( menuMusic, -1 );
                                }
                            }
                            status = MENU;
                            dir = 'x';
                            break;
                        default:
                            dir = 'x';
                            break;
                    }
                    if (dir != 'x' && (status == CLASSIC)) {
                        move(N, dir, gridClassic, &score, -1);
                        spawn(N, gridClassic);
                    }
                    if (dir != 'x' && (status == MUSIC)) {
                        move(N, dir, gridMusic, &musicScore, musicMultiplier);
                        spawn(N, gridMusic);
                    }
                    if (dir != 'x' && (status == NDIM)) {
                        move(dim_grid, dir, gridNdim, &ndimscore, -1);
                        spawn(dim_grid, gridNdim);
                    }
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    clicked = true;
                }
            }
            SDL_GetMouseState(&x, &y);

            if (isVictory(gridClassic, N) && status==CLASSIC && !vicIgnore[0]) { status = VICTORY; previous=1;}
            if (isDefeat(gridClassic, N) && status==CLASSIC) { status = DEFEAT; previous=1; }

            if (isVictory(gridMusic, N) && status==MUSIC && !vicIgnore[1]) { status = VICTORY; previous=2; }
            if (isDefeat(gridMusic, N) && status==MUSIC) { status = DEFEAT; previous=2; }

            if (gridNdim!=NULL) {
                if (isVictory(gridNdim, dim_grid) && status==NDIM && !vicIgnore[2]) { status = VICTORY; previous=3; }
                if (isDefeat(gridNdim, dim_grid) && status==NDIM) { status = DEFEAT; previous=3; }
            }

            switch(status){
                case CLASSIC:
                    classic_display(gridClassic, N, score);
                    break;
                case MENU:
                    menu_display(e, &status, &gridClassic, &gridMusic, &gridNdim,
                                 N, &score, x, y, clicked, &musicScore, &ndimscore, dim_grid);
                    break;
                case MUSIC:
                    music_display(difficulty, gridMusic, N, musicScore, &pressed, &musicMultiplier);
                    break;
                case VICTORY:
                    victory_display(x, y, clicked, &status, previous);
                    break;
                case DEFEAT:
                    defeat_display(x, y, clicked, &status, &gridClassic, &gridMusic,
                                   &gridNdim, N, &score, previous);
                    break;
                case DIFFICULTY:
                    diff_display(x, y, clicked, &status, &difficulty);
                    break;
                case QUIT:
                    quit=true;
                    break;
                case NDIM_SELECT:
                    ndim_select_display(&dim_grid, x, y, clicked, &status, &gridNdim);
                    break;
                case NDIM:
                    ndim_display(gridNdim, dim_grid, ndimscore);
                    //classic_display(gridNdim, dim_grid, ndimscore);
                    break;
                default:
                    break;
            }

        }
    }

    wclose();

    return 0;
}