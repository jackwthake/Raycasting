#include <iostream>
#include <math.h>
#include <string.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>

#include "constants.h"
#include "player.h"


/**
 * @brief Create a randomly generated level
 * 
 * @param size map dimension
 * @param p Reference to the player
 * @return int** 2D Dynamicly allocated level array
 */
int **create_level(int size, Player &p) {
    srand(time(NULL));

    /* create the level array */
    int **level = new int*[size];
    for (int i = 0; i < size; ++i) {
        level[i] = new int[size];

        for (int j = 0; j < size; ++j) {
            level[i][j] = 1;
        }
    }

    int direction = rand() % 3, last_dir = 0; /* holds 0 to 4, represents which direction to move next */
    int floors = 200 + (rand() % 40);
    int x = size / 2, y = size / 2;
    bool player_start_found = false;

    /* lay down floors */
    while (floors > 0) {
        switch (direction) {
            case 0: x += 1; break;
            case 1: x -= 1; break;
            case 2: y += 1; break;
            case 3: y -= 1; break;
        }

        std::clamp(x, 2, size - 2);
        std::clamp(y, 2, size - 2);

        level[x][y] = 0;

        if (rand() % 100 < 25 && !player_start_found) {
            p = Player((double)x + 0.5, (double)y + 0.5);
            player_start_found = true;
        }

        last_dir = direction;
        while(last_dir == direction)
            direction = rand() % 3;
        
        --floors;
    }

    /* ensure bounds */
    for (int i = 0; i < size; ++i) {
        level[0][i] = 1; /* top wall */
        level[size - 1][i] = 1; /* bottom wall */
        level[i][0] = 1; /* left wall */
        level[i][size - 1] = 1; /* right wall */
    }

    return level;
}


int main(int argc, char **argv) {
    UNUSED_SYM(argc);
    UNUSED_SYM(argv);

    sf::RenderWindow win(sf::VideoMode(DISPL_WIDTH * DISPL_SCALE, DISPL_HEIGHT * DISPL_SCALE), "Ray-fun", sf::Style::Close | sf::Style::Titlebar);
    sf::Clock game_clk;

    Player p;
    int **map = create_level(20, p);

    double time = 0.0, acc = 0.0;
    double current_time = game_clk.getElapsedTime().asSeconds();

    /* main loop */
    while (win.isOpen()) {
        double new_time = game_clk.getElapsedTime().asSeconds();
        double frame_time = new_time - current_time;
        double fps = 1.0f / frame_time;
        current_time = new_time;

        acc += frame_time;

        /* tick the game */
        while (acc >= tick_interval) {
            sf::Event e;
            while(win.pollEvent(e)) {
                if (e.type == sf::Event::Closed)
                    win.close();
            }

            p.tick(map, tick_interval);

            acc -= tick_interval;
            time += tick_interval;
        }

        /* Render as much as possible */
        win.clear(sf::Color(0x323250));
        p.render(win, map);
        win.display();
    }

    /* deallocate map */
    for (int i = 0; i < 20; ++i)
        delete map[i];
    delete map;

    return EXIT_SUCCESS;
}