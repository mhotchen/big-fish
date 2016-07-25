#ifndef player_h
#define player_h

#include <SDL2/SDL.h>
#include <vector>
#include "Actor.h"
#include "circle/Circle.h"

class Player : public Actor {
public:
	static constexpr int BEHIND_ANGLE_LOWER   = 135;
	static constexpr int BEHIND_ANGLE_UPPER   = 180 + BEHIND_ANGLE_LOWER;

	Player(int radius, SDL_Point position) : Actor(radius, position) {
		color_ = {0, 50, 100, 255};
		target_ = {0, 0};
		destination_ = {0, 0};
	}

	void handleEvent(SDL_Event* e, SDL_Rect* camera);
	void updateCourse(std::vector<Actor*> actors);

private:
	bool accelerate_ = false;
};

#endif
