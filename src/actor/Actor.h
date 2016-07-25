#ifndef actor_h
#define actor_h

#include <SDL2/SDL.h>
#include <vector>
#include "circle/Circle.h"

class Actor {
public:
	static constexpr float MAX_VELOCITY         = 5.0;
	static constexpr float MIN_VELOCITY         = 1.5;
	static constexpr float ACCELERATION         = 0.1;
	static constexpr int   DESTINATION_DISTANCE = 10000;
	static constexpr float ANGLE_CHANGE         = 6.0;

	Actor(int radius, SDL_Point position) : Actor(radius, position, MIN_VELOCITY) {}
	Actor(int radius, SDL_Point position, float velocity) : circle_(radius, position), velocity_(velocity) {}

    virtual void updateCourse(std::vector<Actor*> actors) = 0;

	void render(SDL_Renderer* renderer, SDL_Rect* camera);
	void move(SDL_Rect* arena);
	void eat(Actor* actor);
	
	bool      isCollision(Actor* actor);
	int       getRadius();
	SDL_Point getPos();

protected:
	static constexpr float PI = 3.14159265;

	Circle    circle_;
	float     velocity_;
	SDL_Point target_, destination_;
    SDL_Color color_ {0, 0, 0, 255};

	float     getAngle(SDL_Point a, SDL_Point b);
	float     distanceSquared(SDL_Point a, SDL_Point b);
	SDL_Point acquireDestination(SDL_Point position, float angle, int distance);
};

#endif
