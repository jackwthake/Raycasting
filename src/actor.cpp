#include "actor.h"

#include "constants.h"

Actor::Actor() : Actor(0.0, 0.0) {}
Actor::Actor(double x, double y) {
    this->x = x;
    this->y = y;
    this->active = true;
}
