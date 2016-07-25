#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <vector>
#include "circle/Circle.h"
#include "Actor.h"
#include "Enemy.h"

void Enemy::updateCourse(std::vector<Actor*> actors) {
	Actor* nearestActor;
	int nearestDistance = 9999999;
	int currentDistance;
	for (int i = 0; i < actors.size(); ++i) {
		if (actors[i] == this) continue;

		currentDistance = abs(distanceSquared(getPos(), actors[i]->getPos()));
		if (currentDistance < nearestDistance) {
			nearestDistance = currentDistance;
			nearestActor = actors[i];
		}
	}

	float angle = getAngle(getPos(), nearestActor->getPos());
	if (getRadius() < nearestActor->getRadius()) angle = -angle; // move away from larger ones

	target_ = acquireDestination(getPos(), angle, DESTINATION_DISTANCE);
	
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

		if      ((diff > 0 && diff <  180) || diff < -180) currentAngle -= ANGLE_CHANGE;
		else if ((diff < 0 && diff > -180) || diff >  180) currentAngle += ANGLE_CHANGE;

		if (currentAngle >  180) currentAngle -= 360;
		if (currentAngle < -180) currentAngle += 360;

		// close enough
		if (abs(currentAngle - targetAngle) < ANGLE_CHANGE) destination_ = target_;
		else destination_ = acquireDestination(circle_.getPos(), currentAngle, DESTINATION_DISTANCE);
	}
}

void Enemy::updateColor(int playerRadius) {
	color_.b = 20;
	color_.r = (playerRadius <= getRadius()) ? 100 : 0;
	color_.g = (playerRadius >  getRadius()) ? 100 : 0;
}
