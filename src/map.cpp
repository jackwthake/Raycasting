#include "map.h"

#include <math.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include "player.h"

Map::Map() : Map(0) {}
Map::Map(int size) {
    this->size = size;

    /* create the level array */
    this->map = new int*[size];
    for (int i = 0; i < size; ++i) {
        this->map[i] = new int[size];

        for (int j = 0; j < size; ++j)
            this->map[i][j] = 1;
    }

    int direction = rand() % 3, last_dir = 0; /* holds 0 to 4, represents which direction to move next */
    int floors = 200 + (rand() % 40);
    int x = this->size / 2, y = this->size / 2;
    bool player_start_found = false;

    /* lay down floors */
    while (floors > 0) {
        switch (direction) {
            case 0: x += 1; break;
            case 1: x -= 1; break;
            case 2: y += 1; break;
            case 3: y -= 1; break;
        }

        x = std::clamp(x, 2, this->size - 2);
        y = std::clamp(y, 2, this->size - 2);

        this->map[x][y] = 0;

        if (rand() % 100 < 25 && !player_start_found) {
            this->player = new Player((double)x + 0.5, (double)y + 0.5);
            player_start_found = true;
        }

        last_dir = direction;
        while(last_dir == direction)
            direction = rand() % 3;
        
        --floors;
    }

    /* ensure bounds */
    for (int i = 0; i < this->size; ++i) {
        this->map[0][i] = 1; /* top wall */
        this->map[this->size - 1][i] = 1; /* bottom wall */
        this->map[i][0] = 1; /* left wall */
        this->map[i][this->size - 1] = 1; /* right wall */
    }
}


Map::~Map() {
    if (this->player)
        delete []this->player;
        
    if (!this->map)
        return;

    for (int i = 0; i < this->size; ++i)
        delete this->map[i];
    delete this->map;
}


void Map::Tick_objs(float dt) {
    this->player->tick(this->map, dt);

    // TODO: Implement some gameobject pool and update all objects
}


void Map::Render(sf::RenderWindow &window) {
    this->player->render(this->map, window);
}