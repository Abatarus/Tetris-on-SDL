#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//Starts up SDL and creates window
_Bool init();

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

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;


int main() {

    if(!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Apply the image
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

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
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;        
}

_Bool loadMedia() {
    //Loading success flag
    _Bool success = 1;

    //Load splash image
    gHelloWorld = SDL_LoadBMP("hello_world.bmp");
    if (gHelloWorld == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n","hello_world.bmp", SDL_GetError());
        success = 0;
    }
    return success;
}

void close_SDL() {
    //Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
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
        //Update the surface
        SDL_UpdateWindowSurface( gWindow );
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