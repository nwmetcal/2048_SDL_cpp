#include "gui.h"
#include <array>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

array<array<unsigned int,3>,12> colors = {32,32,0,
                     					  64,64,0,
                    					  128,128,0,
                    					  170,170,0,
                     					  32,0,32,
                    					  64,0,64,
                    					  128,0,128,
                    					  170,0,170,
                    					  0,32,32,
                 						  0,64,64,
                 						  0,128,128,
                 						  0,170,170};

gui::gui(int width_in, int height_in, int size_in) 
: window(NULL), renderer(NULL), font(NULL), width(width_in), height(height_in), size(size_in) {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Set texture filtering to linear
		if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		window = SDL_CreateWindow("2048!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								 	width, height, SDL_WINDOW_SHOWN);
		if(window == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create vsynced gRenderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(renderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init( imgFlags ) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if(TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	if (!success) {
		exit(1);
	}
	//Open the font
	font = TTF_OpenFont( "/Library/Fonts/Arial.ttf", 50 );
	if(font == NULL) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		exit(1);
	}
	board.resize(size*size);
	cerr << board.size() << endl;
	for (int i = 0; i < board.size(); ++i) {
		SDL_Rect r_temp = {(i%size)*width/size, (i/size)*height/size, width/size, height/size};
		board[i] = r_temp;
	}
}

gui::~gui() {
	//Free loaded images
	text_texture.free();

	//Free global font
	TTF_CloseFont(font);
	font = NULL;

	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void gui::render(std::vector<Tile>& all_tiles) {
	//Clear previous state
    //SDL_SetRenderDrawColor(renderer, 100, 0, 100, 255);
	//SDL_RenderClear(renderer);
	SDL_Color textColor = {0,0,0};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	for (int i = 0; i < board.size(); ++i) {
		int value = all_tiles[i].val;
		if (value > 0) {
			SDL_SetRenderDrawColor(renderer, colors[value%12][0], colors[value%12][1], colors[value%12][2], 255);
			SDL_RenderFillRect(renderer, &board[i]);
			text_texture.load_from_text(to_string((int)pow(2,value)), textColor, renderer, font);
			text_texture.render(((i%size)*(width)/size)-text_texture.getWidth()/2+ width/(size*2),
								(i/size)*(height/size)-text_texture.getHeight()/2 + height/(size*2),
								renderer);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &board[i]);
		}
	}				
        		
    SDL_RenderPresent(renderer);
}
