#include <SFML/Graphics.hpp>

#include "constants.h"
#include "map.h"


int main(int argc, char **argv) {
    UNUSED_SYM(argc);
    UNUSED_SYM(argv);

    sf::RenderWindow win(sf::VideoMode(DISPL_WIDTH * DISPL_SCALE, DISPL_HEIGHT * DISPL_SCALE), "Ray-fun", sf::Style::Close | sf::Style::Titlebar);
    sf::Clock game_clk;

    Map map(20);

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

            map.Tick_objs(tick_interval);

            acc -= tick_interval;
            time += tick_interval;
        }

        /* Render as much as possible */
        win.clear(sf::Color(0x323250));
        map.Render(win);
        win.display();
    }

    return EXIT_SUCCESS;
}