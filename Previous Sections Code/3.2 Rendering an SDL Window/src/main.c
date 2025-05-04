#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;
SDL_Window* window = NULL;      // Make sure they start as no-pointer
SDL_Renderer* renderer = NULL;  // Make sure they start as no-pointer

int initialize_window(void){
    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error Initializing SDL.\n");
        return false;
    }

    int width = 800;
    int height = 600;

    /* Create SDL Window */
    window = SDL_CreateWindow(
        "brbr",                 // Window Title
        SDL_WINDOWPOS_CENTERED, // The X position of the window
        SDL_WINDOWPOS_CENTERED, // The Y position of the window
        width,                  // Window Width
        height,                 // WIndow Height
        0   // Special Flags
    );

    if(!window){
        printf("Error Creating SDL Window.\n");
        return false;
    }

    /* Create SDL renderer*/
    renderer = SDL_CreateRenderer(
        window,                 // Corresponding window
        -1,                     // The default display device
        0                       // Special Flags
    );

    if(!renderer){
        printf("Error Creating SDL Renderer.\n");
        return false;
    }

    return true;
}

void setup(void){
    
}


void process_input(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_QUIT:  // Pressing the "X" at the top-right or top-left of the window.
            is_running = false;
            break;
        case SDL_KEYDOWN: // If a key board button is pressed
            if (event.key.keysym.sym == SDLK_ESCAPE) // If it is the escape button
                is_running = false;    
            break;
    }
}


void update(void){
    
}


void render(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Arguments: (The target renderer, R, G, B, Trasnparency)
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

int main(void){

    /* Initialize SDL Window*/
    is_running = initialize_window();

    setup();

    while (is_running){
        process_input();
        update();
        render();
    }

    return 0;
}