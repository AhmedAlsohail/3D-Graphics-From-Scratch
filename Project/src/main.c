#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "meshes.h"
#include "math.h"

triangle_t triangles_to_render[N_MESH_FACES];

vector2 projectedPoints[N_MESH_VERTICES];

vector3 cameraPos = {0, 0, -5};
vector3 cubeRotation = {0, 0, 0};

float fov_factor = 640;
bool is_running = false;
Uint32 previous_frame_time = 0;
double deltaTime = 1.0 / FPS; // Replace FPS from const to current FPS
void setup(void)
{
    // Alloate the required bytes in memory for the color buffer
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    if (!color_buffer)
    {
        printf("Error: Failed to alloated the color buffer");
    }

    // Setup the SDL texture that is responsible for displaying the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT: // Pressing the "X" at the top-right or top-left of the window.
        is_running = false;
        break;
    case SDL_KEYDOWN:                            // If a key board button is pressed
        if (event.key.keysym.sym == SDLK_ESCAPE) // If it is the escape button
            is_running = false;
        if (event.key.keysym.sym == SDLK_w) // If it is the escape button
            cameraPos.y -= 0.5 * (1.0 / FRAME_TARGET_TIME_ms);
        if (event.key.keysym.sym == SDLK_s) // If it is the escape button
            cameraPos.y += 0.5 * (1.0 / FRAME_TARGET_TIME_ms);
        if (event.key.keysym.sym == SDLK_d) // If it is the escape button
            cameraPos.x += 0.5 * (1.0 / FRAME_TARGET_TIME_ms);
        if (event.key.keysym.sym == SDLK_a) // If it is the escape button
            cameraPos.x -= 0.5 * (1.0 / FRAME_TARGET_TIME_ms);
        if (event.key.keysym.sym == SDLK_UP) // If it is the escape button
            cameraPos.z += 0.5 * (1.0 / FRAME_TARGET_TIME_ms);
        if (event.key.keysym.sym == SDLK_DOWN) // If it is the escape button
            cameraPos.z -= 0.5 * (1.0 / FRAME_TARGET_TIME_ms);
        break;
    }
}

// transform 3d vector to a projected 2d point vector
vector2 projectOrthgraphic(vector3 point)
{
    vector2 projectedPoint = {
        fov_factor * point.x,
        fov_factor * point.y};

    return projectedPoint;
}

vector2 projectPrespective(vector3 point)
{
    vector2 projectedPoint = {
        (fov_factor * point.x) / point.z,
        (fov_factor * point.y) / point.z};

    return projectedPoint;
}

void update(void)
{
    int time_to_wait = FRAME_TARGET_TIME_ms - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME_ms)
        SDL_Delay(time_to_wait);

    previous_frame_time = SDL_GetTicks();

    cubeRotation.x += 1 * deltaTime;
    cubeRotation.y += 1 * deltaTime;

    triangle_t porjectedTriangle;
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        face_t mesh_face = cube_faces[i];

        vector3 face_vertices[3] = {
            cube_vertices[mesh_face.a - 1],
            cube_vertices[mesh_face.b - 1],
            cube_vertices[mesh_face.c - 1],
        };

        for (int j = 0; j < 3; j++)
        {
            vector3 transformed_vertex = face_vertices[j];

            // transformed_vertex = rotateVectorX(transformed_vertex, cubeRotation.x, cubeRotation.y);
            transformed_vertex = rotateVectorY(transformed_vertex, cubeRotation.x, cubeRotation.y);
            // transformed_vertex = rotateVectorZ(transformed_vertex, cubeRotation.x, cubeRotation.y);

            transformed_vertex.x -= cameraPos.x;
            transformed_vertex.y -= cameraPos.y;
            transformed_vertex.z -= cameraPos.z;

            vector2 projectedPoint = projectPrespective(transformed_vertex);

            projectedPoint.x += (window_width / 2);
            projectedPoint.y += (window_height / 2);

            porjectedTriangle.points[j] = projectedPoint;
        }

        triangles_to_render[i] = porjectedTriangle;
    }
}

void draw_line_dda(int x0, int y0, int x1, int y1)
{
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    // Sometimes delta y is greater than delta x, so we need to run the total delta y side length instead of delta x.
    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i < side_length; i++)
    {
        draw_pixel(roundf(current_x), roundf(current_y), 0x008000);
        // draw_rectangle(round(current_x), round(current_y), 3, 3, 0x008000);
        current_x += x_inc;
        current_y += y_inc;
    }
}
void draw_line_bresenham(int x0, int y0, int x1, int y1)
{
    return;
}

void draw_traingle(vector2 point_1, vector2 point_2, vector2 point_3)
{
    draw_line_dda(point_1.x, point_1.y, point_2.x, point_2.y);
    draw_line_dda(point_2.x, point_2.y, point_3.x, point_3.y);
    draw_line_dda(point_3.x, point_3.y, point_1.x, point_1.y);
}

void render(void)
{
    // draw_grid(0xFF000000);

    // loop all projected and render them
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        triangle_t triangle = triangles_to_render[i];

        draw_traingle(triangle.points[0], triangle.points[1], triangle.points[2]);
    }

    render_color_buffer();
    clear_color_buffer(0x000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    /* Initialize SDL Window*/
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}