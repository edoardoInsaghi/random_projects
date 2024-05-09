#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <utility>
#include <SDL2/SDL.h> 

typedef unsigned int uint;

class GameOfLife{
public:

    GameOfLife(int width, int height) : 
        
        width(width), 
        height(height), 
        grid(height, std::vector<int>(width))

        {   
            for(auto &row : grid){
                std::generate(row.begin(), row.end(), [](){return (rand() % 10 == 0) ? 1 : 0;});
            }
        }


    void play(){

        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window *win = SDL_CreateWindow("Game of Life", 100, 100, width, height, SDL_WINDOW_SHOWN);
        SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        SDL_Event event;
        bool run = true;

        while(run){

            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    run = false;
                }
            }

            for(uint i=0; i<height; ++i){
                for(uint j=0; j<width; ++j){
                    (grid[i][j] == 1) ? SDL_SetRenderDrawColor(ren, 255, 255, 255, 255) : SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(ren, j, i);
                }
            }
            SDL_RenderPresent(ren);
            update();
        }

        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();     
    }


    void update(){

        std::vector<std::vector<int>> swap(height, std::vector<int>(width));
            
        for(uint i=0; i<height; ++i){
            for(uint j=0; j<width; ++j){
                swap[i][j] = isAlive(i,j);
            }
        }

        grid = std::move(swap);
    }


    uint isAlive(uint i, uint j){

        uint sum = 0;

        sum = (i > 0 && grid[i-1][j] == 1) ? ++sum : sum; // up
        sum = (i < height-1 && grid[i+1][j] == 1) ? ++sum : sum; // down
        sum = (j > 0 && grid[i][j-1] == 1) ? ++sum : sum; // left
        sum = (j < width-1 && grid[i][j+1] == 1) ? ++sum : sum; // right
        sum = (i > 0 && j > 0 && grid[i-1][j-1] == 1) ? ++sum : sum; // up left
        sum = (i > 0 && j < width-1 && grid[i-1][j+1] == 1) ? ++sum : sum; // up right
        sum = (i < height-1 && j > 0 && grid[i+1][j-1] == 1) ? ++sum : sum; // down left
        sum = (i < height-1 && j < width-1 && grid[i+1][j+1] == 1) ? ++sum : sum; // down right    

        uint alive = 0;
        alive = (grid[i][j] == 0 && sum == 3) ? 1 : alive; // dead with 3 neighoburs alive
        alive = (grid[i][j] == 1 && (sum == 2 || sum == 3)) ? 1 : alive; // alive with 2 or 3 neighbors alive

        return alive;
    }

    void print(){
        for(uint i=0; i<height; ++i){
            for(uint j=0; j<width; ++j){
                std::cout << grid[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    const int height;
    const int width;
    std::vector<std::vector<int>> grid;
};



int main(int argc, char* argv[]){

    int width = 800;
    int height = 600;

    GameOfLife game{width, height};
    game.play();

    return 0;
}
