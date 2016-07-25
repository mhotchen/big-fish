#include <SDL2/SDL.h>
#include "Circle.h"

Circle::Circle(int radius, SDL_Point position) : radius_(radius), position_(position) { };

SDL_Point Circle::getPos() {
	return position_;
}

void Circle::render(SDL_Renderer* renderer, SDL_Rect* camera, SDL_Color color) {
	SDL_Point points[8];

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int r = radius_; r > radius_ - 5; --r) {
		int x = r;
		int y = 0;
		int err = 0;
		while (x >= y) {
			points[0] = SDL_Point {position_.x + x - camera->x, position_.y + y - camera->y};
			points[1] = SDL_Point {position_.x + y - camera->x, position_.y + x - camera->y};
			points[2] = SDL_Point {position_.x - y - camera->x, position_.y + x - camera->y};
			points[3] = SDL_Point {position_.x - x - camera->x, position_.y + y - camera->y};
			points[4] = SDL_Point {position_.x - x - camera->x, position_.y - y - camera->y};
			points[5] = SDL_Point {position_.x - y - camera->x, position_.y - x - camera->y};
			points[6] = SDL_Point {position_.x + y - camera->x, position_.y - x - camera->y};
			points[7] = SDL_Point {position_.x + x - camera->x, position_.y - y - camera->y};
			SDL_RenderDrawPoints(renderer, points, 8);

			y += 1;
			err += 1 + 2*y;
			if (2*(err-x) + 1 > 0) {
				x -= 1;
				err += 1 - 2*x;
			}
		}
	}
}

void Circle::move(SDL_Point position) {
	position_ = position;
}

void Circle::grow(int amount) {
	radius_ += amount;
}

void Circle::shrink(int amount) {
	radius_ -= amount;
}

int Circle::getRadius() {
	return radius_;
}
