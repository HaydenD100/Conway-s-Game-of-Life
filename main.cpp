/*
Made by HaydenD100
GitHub: https://github.com/HaydenD100
Date 2024-01-11
This is an implementation of Conway's game of life a cellular automaton created by John Horton Conway in 1970.
*/

#include <iostream>
#include <SDL.h>
#include <vector>

using namespace std;

//Defines the width and height of the SDL widnow and the size of the grid in pixels 
#define WIDTH 800
#define HEIGHT 600
#define size 10

//this is the struct for the cells which holds information on if the cell if alive and how many alive neighbours the cells has
struct cell {
	int alive = 0;
	int neighbours = 0;
};

//This function takes the grid of cells and the position of the current cell and outputs the number of neighbours that cell has, this function runs through every cell in the update function before updating each cell.
int CheckNeighbour(vector<vector<cell>>&grid, int x, int y)
{
	int neighbours = 0;

	if (x > 0)
		neighbours += grid[x - 1][y].alive;
	if (x < WIDTH)
		neighbours += grid[x + 1][y].alive;
	if(y > 0)
		neighbours += grid[x][y - 1].alive;
	if (y < HEIGHT)
		neighbours += grid[x][y + 1].alive;
	if (x > 0 && y > 0)
		neighbours += grid[x - 1][y - 1].alive;
	if (x > 0 && y < HEIGHT)
		neighbours += grid[x - 1][y + 1].alive;
	if (x < WIDTH && y > 0)
		neighbours += grid[x + 1][y - 1].alive;
	if (x < WIDTH && y < HEIGHT)
		neighbours += grid[x + 1][y + 1].alive;
	return neighbours;
}

//Update function with all of the logic for the cells, this function loops through all the cells in the grid first to update the amount of neighbours each cells have and then to update the state of the cell with the 4 rules of Conway's game of life
void Update(vector<vector<cell>> &grid)
{
	vector<vector<cell>>& grid1 = grid;

	for (int x = 0; x < WIDTH / size - 1; x++)
	{

		for (int y = 0; y < HEIGHT / size - 1; y++)
		{
			grid[x][y].neighbours = CheckNeighbour(grid, x, y);
		}
	}

	for (int x = 0; x < WIDTH / size - 1; x++)
	{

		for (int y = 0; y < HEIGHT / size - 1; y++)
		{
			//if the cell has 2-3 neighbours the cell stays alive
			if (grid[x][y].neighbours > 1 && grid[x][y].neighbours < 4 && grid[x][y].alive == 1) {
				grid[x][y].alive = 1;
			}
			//if the cell is dead but has 3 neighbours the cells is alive due to reprodution 
			else if (grid[x][y].neighbours == 3 && grid[x][y].alive == 0) {
				grid[x][y].alive = 1;
			}
			//else the cell is dead
			else {
				grid[x][y].alive = 0;
			}
		}
	}

	
}

void Draw(SDL_Renderer* renderer, vector<vector<cell>>& grid, bool drawGridOutline) {
	//sets the background colour and then clears the window 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);


	for (int x = 0; x < WIDTH / size; x++)
	{
		for (int y = 0; y < HEIGHT / size; y++)
		{
			//The first rect is the background grid and is used when drawGridOutline is true the second is for the cells and is either black or white depending on if the cell is dead or alive
			SDL_Rect Grid = { x * size, y * size, size, size};
			SDL_Rect rectCell = { x * size + 1, y * size + 1, size - 1, size - 1 };
			if (drawGridOutline == true) 
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &Grid);
			}
			if (grid[x][y].alive == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &rectCell);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rectCell);
			}
			

		}

	}

	//updates the screen with the new drawings in the renderer 
	SDL_RenderPresent(renderer);

}

//Main c++ function 
int main(int argc, char* args[]) {


	vector<vector<cell>> grid;

	
	bool gridVisible = false;
	bool runSimulation = false;
	

	//X and Y postion of the mouse 
	int x, y;

	//These varibles are used to run the simulation at a steady pace using deltatime to calculate how much time has passed, this means the update function will only run after the time timePerUpdateSeconds is set to.
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	double timeTillUpdate = 0;
	double timePerUpdateSeconds = 0.25;


	//initalize all of the cells in the 2d vector 
	for (int x = 0; x < WIDTH / size; x++)
	{
		vector<cell> v1;

		for (int y = 0;y < HEIGHT / size; y++)
		{
			cell newCell;
			v1.push_back(newCell);
		}

		grid.push_back(v1);

	}

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("Conway's Game Of life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Event windowEvent;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Game loop 
	while (true) {

		//Update the x and y varibles of the mouse position and check if the left or right mouse button has been clicked
		Uint32 buttonState = SDL_GetMouseState(&x, &y);
		if (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT) && x >= 0 && x <= WIDTH && y >= 0 && y <= HEIGHT)
			grid[x / size][y / size].alive = 1;
		if (buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT) && x >= 0 && x <= WIDTH && y >= 0 && y <= HEIGHT)
			grid[x / size][y / size].alive = 0;
		
		
		//Update the timeTillUpdate varible by calcualting deltaTime and then adding to timeTillUpdate
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		timeTillUpdate += deltaTime;
		
		
		if (SDL_PollEvent(&windowEvent)) {
			if (SDL_QUIT == windowEvent.type) break;

			if (SDL_KEYDOWN == windowEvent.type) 
			{
				if (SDLK_g == windowEvent.key.keysym.sym) 
				{
					gridVisible = !gridVisible;
				}
				if (SDLK_p == windowEvent.key.keysym.sym)
				{
					runSimulation = !runSimulation;
				}
			}


		}

		//Check if the time that has passed since the last update has passed and if so update the grid.
		if (timeTillUpdate / 1000 >= timePerUpdateSeconds && runSimulation)
		{
			timeTillUpdate = 0;
			Update(grid);
		}
		
		
		Draw(renderer,grid, gridVisible);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return 0;
}

