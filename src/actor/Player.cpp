#include <cmath>
#include <SDL2/SDL.h>
#include <vector>
#include "circle/Circle.h"
#include "Player.h"

void Player::handleEvent(SDL_Event* e, SDL_Rect* camera) {
	if (e->type == SDL_MOUSEBUTTONUP) {
		accelerate_ = false;
		target_ = {0, 0};
	}

	if (e->type == SDL_MOUSEBUTTONDOWN) {
		accelerate_ = true;
	}

	int x, y;
	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		float angle = getAngle(circle_.getPos(), SDL_Point {x + camera->x, y + camera->y});
		target_ = acquireDestination(circle_.getPos(), angle, DESTINATION_DISTANCE);
	}
}

void Player::updateCourse(std::vector<Actor*>) {
	if (destination_.x == 0 && destination_.y == 0 &&
		target_.x  == 0 && target_.y  == 0
	) {
		return;
	}

	// if current destination has never been set before go directly to target
	if (destination_.x == 0 && destination_.y == 0) destination_ = target_;

	/*
	 * Recalculate current destination
	 */

	if (target_.x != 0 && target_.y != 0 &&
		target_.x != destination_.x && target_.y != destination_.y
	) {
		float targetAngle = getAngle(circle_.getPos(), target_);

		float currentAngle = getAngle(circle_.getPos(), destination_);
		float diff = currentAngle - targetAngle;

		if      ((diff > 0 && diff <  BEHIND_ANGLE_LOWER) || diff < -BEHIND_ANGLE_UPPER) currentAngle -= ANGLE_CHANGE;
		else if ((diff < 0 && diff > -BEHIND_ANGLE_LOWER) || diff >  BEHIND_ANGLE_UPPER) currentAngle += ANGLE_CHANGE;

		if (
			(diff < -BEHIND_ANGLE_LOWER && diff > -BEHIND_ANGLE_UPPER) ||
			(diff >  BEHIND_ANGLE_LOWER && diff <  BEHIND_ANGLE_UPPER)
		) {
			velocity_ -= ACCELERATION;
			accelerate_ = false;
			if (velocity_ <= MIN_VELOCITY) {
				destination_ = target_;
				currentAngle = targetAngle;
				diff = 0;
				accelerate_ = true;
				velocity_ = MIN_VELOCITY;
			}
		}

		if (currentAngle >  180) currentAngle -= 360;
		if (currentAngle < -180) currentAngle += 360;

		// close enough
		if (abs(currentAngle - targetAngle) < ANGLE_CHANGE) destination_ = target_;
		else destination_ = acquireDestination(circle_.getPos(), currentAngle, DESTINATION_DISTANCE);
	}

	/*
	 * Accelerate
	 */

	if (accelerate_) {
		velocity_ += ACCELERATION;

		if (velocity_ > MAX_VELOCITY) velocity_ = MAX_VELOCITY;
	}
}
