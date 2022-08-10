#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>
#include <SFML/Graphics.hpp>

#include "actor.h"

class Player : public Actor {
    public:
        Player();
        Player(double x, double y);

        void tick(int **map, float delta) override;
        void render(int **map, sf::RenderWindow &window) override;
    private:
        double dir_x = -1, dir_y = 0;
        double plane_x = 0, plane_y = 0.66;
};

#endif