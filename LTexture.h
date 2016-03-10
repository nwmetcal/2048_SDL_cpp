#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class LTexture {
public:
	//Initializes variables
	LTexture();
	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool load_from_file(std::string path, SDL_Renderer *renderer);

	//Creates image from font string
	bool load_from_text(std::string textureText, SDL_Color textColor, 
						SDL_Renderer *renderer, TTF_Font *font);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(unsigned char red, unsigned char green, unsigned char blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(unsigned char alpha);
	
	//Renders texture at given point
	void render(int x, int y, SDL_Renderer *renderer, SDL_Rect* clip = NULL,
				double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();
private:
	//The actual hardware texture
	SDL_Texture* m_texture;
	//Image dimensions
	int width;
	int height;
};
