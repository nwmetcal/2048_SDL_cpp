#include "LTexture.h"
#include <vector>
#include "Game.h"

class gui {
public:
	gui(int width_in, int height_in, int size_in);
	~gui();
	void render(std::vector<Tile>& all_tiles);

private:
	SDL_Window* window;

	SDL_Renderer* renderer;

	TTF_Font *font;

	LTexture text_texture;

	std::vector<SDL_Rect> board;
	int fps;
	int width;
	int height;
	int size;
};
