#ifndef circle_h
#define circle_h

#include <SDL2/SDL.h>

class Circle {
public:
	Circle(int radius, SDL_Point position);
	SDL_Point getPos();
	void render(SDL_Renderer* renderer, SDL_Rect* camera, SDL_Color color);
	void move(SDL_Point position);
	void grow(int amount);
	void shrink(int amount);
	int getRadius();
private:
	int radius_;
	SDL_Point position_;
};

#endif
