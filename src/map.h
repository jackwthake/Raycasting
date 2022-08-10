#ifndef __MAP_H__
#define __MAP_H__

#include <SFML/Graphics/RenderWindow.hpp>

class Map {
    public:
        Map();
        Map(int size);
        ~Map();

        void Tick_objs(float dt);
        void Render(sf::RenderWindow &window);
    private:
        class Player *player;
        int **map;
        int size;
};

#endif