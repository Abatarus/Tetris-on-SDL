#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//Starts up SDL and creates window
_Bool init();


SDL_Texture* loadTexture(char* path);

//Load media
_Bool loadMedia();

//Frees media and shuts down SDL
void close_SDL();

struct GameStruct {
    _Bool quit;
};
//Game main loop
void gameLoop();

//event handling 
void handleEvents(struct GameStruct * game);

//The window we`be rendering to
SDL_Window* gWindow = NULL;

//The window render
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;


int main() {

    if(!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            gameLoop();
        }
    }

    //Free resources and close SDL
    close_SDL();
    exit(0);
}

_Bool init() {
    //Initialization flag
    _Bool success = 1;
    
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_could not initialize! SDL_Error: %s.\n", SDL_GetError());
    } else {
        //Create window
        gWindow = SDL_CreateWindow("SDL works", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s.\n", SDL_GetError());
            success = 0;
        } else {
            //Create renderer for windows
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = 0;
            } else {
                //Initialize render color
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = 0;
                }
            }
        }
    }
    return success;        
}

_Bool loadMedia() {
    //Loading success flag
    _Bool success = 1;

    //Load splash image
    gTexture = loadTexture("tetris_sprites.png");
    if (gTexture == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n","tetris_sprites.png", SDL_GetError());
        success = 0;
    }
    return success;
}

void close_SDL() {
    //Deallocate surface
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void gameLoop() {
    struct GameStruct game;
    //Main loop flag
    game.quit = 0;
    while (!game.quit) {
        handleEvents(&game);
        
        //Clear screen
        SDL_RenderClear(gRenderer);

        //Render texture to screen
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        //Update screen
        SDL_RenderPresent(gRenderer);
    }
}


void handleEvents(struct GameStruct * game) {
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e)) {
        //User requests quit
        if(e.type == SDL_QUIT) {
            game->quit = 1;
        }
    }
}

SDL_Texture* loadTexture(char* path) {
    //The final teture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
    } else {
        //Convert texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL) {
            printf("Unable to create texture %s! SDL_Error: %s\n", path, SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}