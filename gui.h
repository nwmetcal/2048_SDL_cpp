#include "LTexture.h"
#include <vector>
#include "Game.h"

class gui {
public:
	gui(int width_in, int height_in, int size_in);
	~gui();
	void render(std::vector<Tile>& all_tiles);
	SDL_Rect render_start();
	void render_game_over(std::string score, std::vector<Tile>& all_tiles);
	void set_width(int width_in);
	void set_height(int height_in);
	void update_tiles();
private:
	SDL_Window* window;

	SDL_Renderer* renderer;

	TTF_Font *font;

	LTexture text_texture;

	std::vector<SDL_Rect> board;
	std::vector<SDL_Rect> board_back;
	SDL_Rect background;
	SDL_Color textColor;
	int fps;
	int width;
	int height;
	int size;
};
