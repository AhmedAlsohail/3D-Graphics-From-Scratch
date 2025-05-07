#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;
SDL_Window* window = NULL;      // Make sure they start as no-pointer
SDL_Renderer* renderer = NULL;  // Make sure they start as no-pointer

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

int initialize_window(void){
    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error Initializing SDL.\n");
        return false;
    }

    // use SDL query to know what is the full screen height and width
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    //window_width = display_mode.w;
    //window_height = display_mode.h;

    /* Create SDL Window */
    window = SDL_CreateWindow(
        "brbr",                 // Window Title
        SDL_WINDOWPOS_CENTERED, // The X position of the window
        SDL_WINDOWPOS_CENTERED, // The Y position of the window
        window_width,                  // Window Width
        window_height,                 // WIndow Height
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
    
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

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

void clear_color_buffer(void){
    for(int y = 0; y < window_height; y++){ // Rows
        for(int x = 0; x < window_width; x++){ // Columns
            color_buffer[(window_width*y + x)] = 0xFF000000;
        }
    }
}

void draw_grid(void){
    for(int y = 0; y < window_height; y++){ // Rows
        for(int x = 0; x < window_width; x++){ // Columns
            if(x % 10 == 0 || y % 10 == 0)
                color_buffer[(window_width*y + x)] = 0xFF000000;
            else
                color_buffer[(window_width*y + x)] = 0xFFFFFFFF;
        }
    }
}

void draw_rectangle(int xPos, int yPos, int width, int height, uint32_t color){
    for(int x = xPos; x < xPos+width; x++){ // Rows
        for(int y = yPos; y < yPos+height; y++){ // Columns
            color_buffer[(window_width*y + x)] = color;
        }
    }
}

void render_color_buffer(void){
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );

    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
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

void destroy_window(void){
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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