#include "gui.h"
#include <array>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

array<array<unsigned int,3>,12> colors = {64,64,0,
                     					  0xDC,0x14,0x3C,
                    					  128,128,0,
                    					  0xFF,0x45, 0,
                     					  32,0,32,
                    					  64,0,64,
                    					  128,0,128,
                    					  0x7B, 0x68, 0xEE,
                    					  0,0,0x8B,
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
								 	width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
	font = TTF_OpenFont( "/Library/Fonts/Arial.ttf", 48 );
	if(font == NULL) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		exit(1);
	}
	board.resize(size*size);
	board_back.resize(size*size);
	update_tiles();
	SDL_Color temp_color = {255,255,255};
	textColor = temp_color;
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

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xC8, 0xCB, 255);
	SDL_RenderClear(renderer);
	SDL_RenderFillRect(renderer, &background);
	for (int i = 0; i < board.size(); ++i) {
		int value = all_tiles[i].val;
		if (value > 0) {
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xE4, 0xB5, 255);
			SDL_RenderFillRect(renderer, &board_back[i]);

			SDL_SetRenderDrawColor(renderer, colors[value%12][0], colors[value%12][1], colors[value%12][2], 255);
			SDL_RenderFillRect(renderer, &board[i]);
			text_texture.load_from_text(to_string((int)pow(2,value)), textColor, renderer, font);
			text_texture.render(((i%size)*(width)/size)-text_texture.getWidth()/2+ width/(size*2),
								(i/size)*(height/size)-text_texture.getHeight()/2 + height/(size*2),
								renderer);
		}
	}		
        		
    SDL_RenderPresent(renderer);
}

SDL_Rect gui::render_start() {
	SDL_RenderClear(renderer);

	text_texture.load_from_text("2048!", textColor, renderer, font);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &background);
	text_texture.render(width/2 - text_texture.getWidth()/2,
						height/4 - text_texture.getHeight(),
						renderer);

	text_texture.load_from_text("Start!", textColor, renderer, font);
	int box_width = 2*text_texture.getWidth();
	int box_height = 3*text_texture.getHeight();
	SDL_Rect r_temp = {width/2 - box_width/2, height/2 - box_height/2,
					   box_width, box_height};

	SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 255);
	SDL_RenderFillRect(renderer, &r_temp);
	text_texture.render(width/2 - text_texture.getWidth()/2,
						height/2 - text_texture.getHeight()/2,
						renderer);
	SDL_RenderPresent(renderer);
	return r_temp;
}

void gui::render_game_over(string score, std::vector<Tile>& all_tiles) {
	SDL_RenderClear(renderer);
	render(all_tiles);

	text_texture.load_from_text("Game Over!", textColor, renderer, font);
	int box_width = 2*text_texture.getWidth();
	int box_height = 3*text_texture.getHeight();
	SDL_Rect r_temp = {width/2 - box_width/2, height/2 - box_height/2,
					   box_width, box_height};
	
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xD7, 0, 255);
	SDL_RenderFillRect(renderer, &r_temp);
	text_texture.render(width/2 - text_texture.getWidth()/2,
						height/2 - text_texture.getHeight(),
						renderer);

	text_texture.load_from_text("Your Score: "+score, textColor, renderer, font);
	text_texture.render(width/2 - text_texture.getWidth()/2,
						height/2 + text_texture.getHeight()/4,
						renderer);
	SDL_RenderPresent(renderer);
}

void gui::set_width(int width_in) {
	width = width_in;
}

void gui::set_height(int height_in) {
	height = height_in;
}

void gui::update_tiles() {
	SDL_Rect r_background = {0, 0, width, height};
	background = r_background;
	for (int i = 0; i < board.size(); ++i) {
		SDL_Rect r_temp = {(i%size)*width/size + 10, (i/size)*height/size + 10, (width/size) - 20, (height/size) - 20};
		board[i] = r_temp;
	}
	for (int i = 0; i < board_back.size(); ++i) {
		SDL_Rect r_temp = {(i%size)*width/size + 5, (i/size)*height/size + 5, (width/size) - 10, (height/size) - 10};
		board_back[i] = r_temp;
	}
}
