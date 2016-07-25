#ifndef enemy_h
#define enemy_h

#include <SDL2/SDL.h>
#include <vector>
#include "Actor.h"
#include "circle/Circle.h"

class Enemy : public Actor {
public:
	Enemy(int radius, SDL_Point position, float velocity) : Actor(radius, position, velocity) {}
	void updateCourse(std::vector<Actor*> actors);
    void updateColor(int playerRadius);
};

#endif
