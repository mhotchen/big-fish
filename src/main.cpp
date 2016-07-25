#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "actor/Actor.h"
#include "actor/Enemy.h"
#include "actor/Player.h"

const int SCREEN_WIDTH  = 2500;
const int SCREEN_HEIGHT = 1200;

bool init();
bool loadMedia();
void close();

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* bg = NULL;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Big Fish", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could now be created! Error: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Unable to initialize renderer! Error: %s\n", SDL_GetError());
		return false;
	}

	IMG_Init(IMG_INIT_JPG);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	return true;
}

bool loadMedia() {
	SDL_Surface* loaded = IMG_Load("assets/bg-level.jpg");
	SDL_Surface* converted = SDL_ConvertSurface(loaded, SDL_GetWindowSurface(window)->format, 0);
	bg = SDL_CreateTextureFromSurface(renderer, converted);
	SDL_FreeSurface(converted);
	SDL_FreeSurface(loaded);
	return true;
}

void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	renderer = NULL;
	window = NULL;

	SDL_Quit();
}

int main (int argc, char* args[]) {
	if (!init()) {
		return 1;
	}

	if (!loadMedia()) {
		return 1;
	}

	SDL_Event e;

	Player player(60, SDL_Point {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
	SDL_Rect arena {0, 0, 4044, 1488};

	SDL_Rect camera {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	std::vector<Enemy> enemies;
	enemies.reserve(30);

	std::srand(std::time(0) + SDL_GetTicks());
	for (int i = 0; i < 30; ++i) {
		enemies.emplace_back(
			10 + (rand() % 100),
			SDL_Point {
				arena.x + (rand() % arena.w),
				arena.y + (rand() % arena.w)
			},
			(15 + (rand() % 10)) / 10.0
		);
	}

	std::vector<Actor*> actors;
	actors.reserve(enemies.size() + 1);
	for (int i = 0; i < enemies.size(); ++i) {
		actors.push_back(&enemies[i]);
	}

	actors.push_back(&player);

	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
			else player.handleEvent(&e, &camera);
		}

		for (int i = 0; i < actors.size(); ++i) {
			actors[i]->move(&arena);

			for (int j = 0; j < actors.size(); ++j) {
				if (actors[i] == actors[j]) continue;

				if (actors[i]->isCollision(actors[j])) {
					actors[i]->eat(actors[j]);
				}
			}

			actors[i]->updateCourse(actors);

			if (actors[i]->getRadius() <= 2) {
				actors.erase(actors.begin() + i);
				--i;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg, &camera, NULL);

		camera.x = player.getPos().x - camera.w / 2;
		camera.y = player.getPos().y - camera.h / 2;
		if (camera.x < arena.x) camera.x = arena.x;
		if (camera.y < arena.y) camera.y = arena.y;
		if (camera.x > arena.w - camera.w) camera.x = arena.w - camera.w;
		if (camera.y > arena.h - camera.h) camera.y = arena.h - camera.h;

		player.render(renderer, &camera);
		for (int i = 0; i < enemies.size(); ++i) {
			enemies[i].updateColor(player.getRadius());
			enemies[i].render(renderer, &camera);
		}

		SDL_RenderPresent(renderer);
	}


	close();
	return 0;
}
