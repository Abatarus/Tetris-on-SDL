#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//Starts up SDL and creates window
_Bool init();


SDL_Surface* loadSurface(char* path);

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
SDL_Surface* gBackScreenSurface = NULL;


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
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = 0;
            }
            else
            {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
    }
    return success;        
}

_Bool loadMedia() {
    //Loading success flag
    _Bool success = 1;

    //Load splash image
    gBackScreenSurface = loadSurface("loaded.png");
    if (gBackScreenSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n","loaded.png", SDL_GetError());
        success = 0;
    }
    return success;
}

void close_SDL() {
    //Deallocate surface
    SDL_FreeSurface(gBackScreenSurface);
    gBackScreenSurface = NULL;

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
        //Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        //Apply the image
        SDL_BlitScaled(gBackScreenSurface, NULL, gScreenSurface, &stretchRect);
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

SDL_Surface* loadSurface(char* path) {
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
    } else {
        //Convert surface to screen fornat
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if(optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL_Error: %s\n", path, SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}