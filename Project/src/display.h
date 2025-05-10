#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window* window;      // Make sure they start as no-pointer
extern SDL_Renderer* renderer;  // Make sure they start as no-pointer

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;


bool initialize_window(void);

void render_color_buffer(void);
void clear_color_buffer(void);

void draw_grid(void);
void draw_rectangle(int xPos, int yPos, int width, int height, uint32_t color);

void destroy_window(void);
#endif