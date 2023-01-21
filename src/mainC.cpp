#include <TestArea.hpp>

#define WIDTH 500
#define HEIGHT 500

void AddLine(TestArea &screen, Vector2 const &pos)
{
	static Vector2 last;
	static bool first = false;

	screen.DrawCircle(pos, 3, color::blue);

	if (first == false)
		first = true;
	else
		screen.DrawLine(last, pos);

	last = pos;
	screen.UpdateSurface();
}

void Event(TestArea &screen)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			exit(0);
		}

		if ((e.type == SDL_KEYDOWN) &&
		    (e.key.keysym.sym == SDLK_ESCAPE)) {
			exit(0);
		}

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			mouseX -= WIDTH / 2;
			mouseY = HEIGHT / 2 - mouseY;
			AddLine(screen, Vector2(mouseX, mouseY));
			std::cout << mouseX << " " << mouseY << std::endl;
		}
	}
}

// this program created for create map. recomended using "./out > map.txt"

int main()
{
	TestArea screen(WIDTH, HEIGHT);
	while (1) {
		Event(screen);
	}
}