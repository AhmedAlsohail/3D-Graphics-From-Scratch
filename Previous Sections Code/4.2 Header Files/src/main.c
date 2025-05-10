#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

bool is_running = false;

void setup(void){
    // Alloate the required bytes in memory for the color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

    if(!color_buffer){
        printf("Error: Failed to alloated the color buffer");
    }

    // Setup the SDL texture that is responsible for displaying the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
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

    render_color_buffer();

    clear_color_buffer();

    draw_grid();

    draw_rectangle(100, 100, 250, 150, 0xFF00FFFF);


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

    destroy_window();

    return 0;
}