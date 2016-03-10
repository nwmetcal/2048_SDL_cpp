#include "LTexture.h"

using namespace std;

LTexture::LTexture() : m_texture(NULL), width(0), height(0) {}

LTexture::~LTexture() {
	free();
}

bool LTexture::load_from_file(string path, SDL_Renderer *renderer) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* new_texture = NULL;

	//Load image at specified path
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if(loaded_surface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//Color key image
		SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
		if(new_texture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			//Get image dimensions
			width = loaded_surface->w;
			height = loaded_surface->h;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loaded_surface);
	}
	//Return success
	m_texture = new_texture;
	return m_texture != NULL;
}

bool LTexture::load_from_text(std::string textureText, SDL_Color textColor,
							  SDL_Renderer *renderer, TTF_Font *font) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if(text_surface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
        m_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		if(m_texture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else {
			//Get image dimensions
			width = text_surface->w;
			height = text_surface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface(text_surface);
	}
	//Return success
	return m_texture != NULL;}

void LTexture::free() {
	//Free texture if it exists
	if(m_texture != NULL) {
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		width = 0;
		height = 0;
	}
}

void LTexture::setColor(unsigned char red, unsigned char green, unsigned char blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(m_texture, blending);
}

//Set alpha modulation
void LTexture::setAlpha(unsigned char alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(m_texture, alpha);
}
	
//Renders texture at given point
void LTexture::render(int x, int y, SDL_Renderer *renderer, SDL_Rect* clip,
			double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect render_quad = {x, y, width, height};

	//Set clip rendering dimensions
	if(clip != NULL) {
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, m_texture, clip, &render_quad, angle, center, flip);
}

int LTexture::getWidth() {
	return width;
}

int LTexture::getHeight() {
	return height;
}
