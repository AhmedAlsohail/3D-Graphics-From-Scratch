#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

//////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
//////////////////////////////////////////////////////////////////////
#define N_POINTS (9 * 9 * 9)
vector3 cube_points[N_POINTS]; // 9x9x9 cube
vector2 projectedPoints[N_POINTS];

vector3 cameraPos = {0, 0, -5};
vector3 cubeRotation = {0, 0, 0};

float fov_factor = 640;
bool is_running = false;
Uint32 previous_frame_time = 0;
double deltaTime = 1.0/FPS;
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

    // 
    int point_counter = 0;
    for (float x = -1; x <= 1; x+=0.25){
        for (float y = -1; y <= 1; y+=0.25){
            for (float z = -1; z <= 1; z+=0.25){
                vector3 new_point = {x, y, z};
                cube_points[point_counter++] = new_point;
            }
        }
    }
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
            if (event.key.keysym.sym == SDLK_w) // If it is the escape button
                cameraPos.y -= 0.5 * (1.0/FRAME_TARGET_TIME_ms);
            if (event.key.keysym.sym == SDLK_s) // If it is the escape button
                cameraPos.y += 0.5* (1.0/FRAME_TARGET_TIME_ms);
            if (event.key.keysym.sym == SDLK_d) // If it is the escape button
                cameraPos.x += 0.5* (1.0/FRAME_TARGET_TIME_ms);
            if (event.key.keysym.sym == SDLK_a) // If it is the escape button
                cameraPos.x -= 0.5* (1.0/FRAME_TARGET_TIME_ms);
            if (event.key.keysym.sym == SDLK_UP) // If it is the escape button
                cameraPos.z += 0.5* (1.0/FRAME_TARGET_TIME_ms);
            if (event.key.keysym.sym == SDLK_DOWN) // If it is the escape button
                cameraPos.z -= 0.5* (1.0/FRAME_TARGET_TIME_ms);
            break;
    }
}

// transform 3d vector to a projected 2d point vector
vector2 projectOrthgraphic (vector3 point){
    vector2 projectedPoint = {
         fov_factor * point.x,
         fov_factor * point.y
    };

    return projectedPoint;
}

vector2 projectPrespective (vector3 point){
    vector2 projectedPoint = {
         (fov_factor * point.x) / point.z,
         (fov_factor * point.y) / point.z
    };

    return projectedPoint;
}

void update(void){
   int time_to_wait = FRAME_TARGET_TIME_ms - (SDL_GetTicks() - previous_frame_time);

   if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME_ms)
        SDL_Delay(time_to_wait);
    
    previous_frame_time = SDL_GetTicks();
    
    //cubeRotation.x += 0.001;
    cubeRotation.y += 1 * deltaTime;

    for (int i=0; i< N_POINTS; i++){
        vector3 point = cube_points[i];

        vector3 transformPoint = rotateVectorY(point, cubeRotation.x, cubeRotation.y);

        transformPoint.x -= cameraPos.x;
        transformPoint.y -= cameraPos.y;
        transformPoint.z -= cameraPos.z;

        // project the current point
        vector2 projectedPoint = projectPrespective(transformPoint);
        // Save the projected 2D vector in the array of projected points
        projectedPoints[i] = projectedPoint;
    }
}

void render(void){
    //draw_grid(0xFF000000);

    // loop all projected and render them
    for (int i=0; i<N_POINTS; i++){
        vector2 projectedPoint = projectedPoints[i];
        draw_rectangle(projectedPoint.x + (window_width / 2),
                        projectedPoint.y + (window_height / 2), 
                        4,
                        4,
                        0xFFFFFFF
                    );
    }

    render_color_buffer();
    clear_color_buffer(0x000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]){
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