#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include <time.h>

SDL_Window* window;
SDL_Renderer* renderer;

#define size 10
#define width 50
#define height 50

int board[width][height];

void render_cells(bool reset){
    if (reset){
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                board[i][j] = 0; /* Everything is not visited */
            }
        }
    }

    SDL_RenderClear(renderer);

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            SDL_Rect cell = {j * size, i * size, size, size};
            if (board[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 171, 255, 0);
            }
            SDL_RenderFillRect(renderer, &cell);
        }
    }
    SDL_RenderPresent(renderer);
}

void fisher_yates_shuffle(int* arr, int n){
    if (n <= 1) return;
    for (int i = 0; i < n - 1; i++){
        int j = i + rand() / (RAND_MAX / (n - 1) + 1);
        int t = arr[j];
        arr[j] = arr[i];
        arr[i] = t;
    }
}

void backtrack_recursive(int x, int y){
    
    /* Mark the current node as visited */
    board[y][x] = 1;
    
    /* Visit the next cell using these directions */
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    /* Shuffle the directions to randomize */
    fisher_yates_shuffle((int*)directions, 4);

    /* Loop through the directions and repeat the process */
    for (int i = 0; i < 4; i++){

        int dx = directions[i][0];
        int dy = directions[i][1];
        
        int nx = x + 2 * dx;
        int ny = y + 2 * dy;
        
        /* If the next node exists, and it's not visited, then we go there. */
        if (0 <= nx && nx < width && 0 <= ny && ny < height && board[ny][nx] == 0){
            /* Switch to this node, and backtrack again */
            board[y + dy][x + dx] = 1;
            backtrack_recursive(nx, ny);
            //printf("%d\n", nx);
        }
    }
    
}

void generate(){
    /* Reset the entire maze, all nodes being unvisited */
    render_cells(true);
    /* Start from top most left corner */
    backtrack_recursive(0, 0);
    /* Render the maze again, but no resetting */
    render_cells(false);
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    if (SDL_Init(0x00000020u) != 0) return 1;
    window = SDL_CreateWindow("Maze renderer", 0x1FFF0000u, 0x1FFF0000u, 0b111110100, 0x1f4, 0b10*2);
    renderer = SDL_CreateRenderer(window, -1, 2);

    SDL_Event event;
    render_cells(true); /* Reset and prepare */

    while (true) {
        SDL_WaitEvent(&event);

        if (event.type == 256) break;
        else if (event.type == 768 && event.key.keysym.sym == 32) generate(); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}