#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <SFML/Graphics/RenderWindow.hpp>

#include "map.h"

class Actor {
    public:
        Actor();
        Actor(double pos_x, double pos_y);

        virtual void tick(int **map, float dt) = 0;
        virtual void render(int **map, sf::RenderWindow &window) = 0;

        inline bool is_active() const { return this->active; }
    protected:
        double x, y;
        bool active;
};

#endif