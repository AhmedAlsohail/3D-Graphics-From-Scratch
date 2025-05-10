#include "display.h"

SDL_Window* window = NULL;      // Make sure they start as no-pointer
SDL_Renderer* renderer = NULL;  // Make sure they start as no-pointer

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void){
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

void render_color_buffer(void){
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );

    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
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

void destroy_window(void){
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}