#include "circle/Circle.h"
#include "Actor.h"

void Actor::eat(Actor *actor) {
    if (actor->circle_.getRadius() < circle_.getRadius()) {
        circle_.grow(1);
        actor->circle_.shrink(2);
    } else {
        circle_.shrink(2);
        actor->circle_.grow(1);
    }
}

void Actor::render(SDL_Renderer* renderer, SDL_Rect* camera) {
    circle_.render(renderer, camera, color_);
}

bool Actor::isCollision(Actor* actor) {
    int totalRadiusSquared = (circle_.getRadius() + actor->circle_.getRadius()) * (circle_.getRadius() + actor->circle_.getRadius());

    return distanceSquared(circle_.getPos(), actor->circle_.getPos()) < totalRadiusSquared;
}

SDL_Point Actor::getPos() {
    return circle_.getPos();
}

int Actor::getRadius() {
    return circle_.getRadius();
}

void Actor::move(SDL_Rect *arena) {
    if (destination_.x == 0 && destination_.y == 0 && target_.x == 0 && target_.y == 0) return;

    int velocityRounded = lround(velocity_);
    SDL_Point newLocation = acquireDestination(circle_.getPos(), getAngle(circle_.getPos(), destination_), velocityRounded);
    circle_.move(newLocation);

    if (circle_.getPos().y + circle_.getRadius() > arena->y + arena->h) {
        circle_.move(SDL_Point {circle_.getPos().x, arena->y + arena->h - circle_.getRadius()});
        target_.y = -target_.y;
        destination_.y = -destination_.y;
    }

    if (circle_.getPos().y - circle_.getRadius() < arena->y) {
        circle_.move(SDL_Point {circle_.getPos().x, arena->y + circle_.getRadius()});
        target_.y = -target_.y;
        destination_.y = -destination_.y;
    }

    if (circle_.getPos().x + circle_.getRadius() > arena->x + arena->w) {
        circle_.move(SDL_Point {arena->x + arena->w - circle_.getRadius(), circle_.getPos().y});
        target_.x = -target_.x;
        destination_.x = -destination_.x;
    }

    if (circle_.getPos().x - circle_.getRadius() < arena->x) {
        circle_.move(SDL_Point {arena->x + circle_.getRadius(), circle_.getPos().y});
        target_.x = -target_.x;
        destination_.x = -destination_.x;
    }
}


float Actor::getAngle(SDL_Point a, SDL_Point b) {
    float xDiff = a.x - b.x;
    float yDiff = a.y - b.y;
    return atan2(yDiff, xDiff) * (180.0 / 3.14159265);
}

SDL_Point Actor::acquireDestination(SDL_Point currentPosition, float angle, int distance) {
    // inversion of distance because pixels and angles increment oppositely
    int x = cos(angle * (3.14159265 / 180)) * -distance;
    int y = sin(angle * (3.14159265 / 180)) * -distance;

    return SDL_Point {x + currentPosition.x, y + currentPosition.y};
}

float Actor::distanceSquared(SDL_Point a, SDL_Point b) {
    int x = b.x - a.x;
    int y = b.y - a.y;
    return x*x + y*y;
}
