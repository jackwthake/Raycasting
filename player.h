#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>
#include <SFML/Graphics.hpp>

class Player {
    public:
        Player();
        Player(double x, double y);

        void tick(int **map, float delta);
        void render(sf::RenderWindow &window, int **map);
    private:
        double x, y;
        double dir_x = -1, dir_y = 0;
        double plane_x = 0, plane_y = 0.66;
};

#endif