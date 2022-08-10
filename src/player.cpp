#include "player.h"

#include <math.h>
#include <iostream>

#include "constants.h"


/**
 * @brief Calculates the fog adjusted color using the distance traveled by the ray
 * 
 * @param distance the distance traveled to hit a wall
 * @param fog_factor the cutoff point of the fog (x * pi) / 2 = cutoff
 * @param input the original colour of the wall
 * @param fog_col the color of the fog
 */
static void get_fog_adjusted_color(double distance, int fog_factor, sf::Color &input, sf::Color fog_col) {
    double input_perc = cos(distance / fog_factor); /* smooth decrease from 1 to 0 */
    double fog_perc = 1 - input_perc; /* smooth increase from 0 to 1 */

    /* (fog_factor * pi / 2) is the first maxima / minima, this avoids the periodic nature of trig functions */
    if (distance > (fog_factor * M_PI) / 2) {
        input_perc = 0;
        fog_perc = 0;
    }

    /*  mix colors */
    input.r = (input.r * input_perc) + (fog_col.r * fog_perc);
    input.g = (input.g * input_perc) + (fog_col.g * fog_perc);
    input.b = (input.b * input_perc) + (fog_col.b * fog_perc);
}


/**
 * @brief Construct a new Player
 */
Player::Player() : Player(0, 0) { }
Player::Player(double x, double y) {
    this->x = x;
    this->y = y;
}


/**
 * @brief Tick the player, check for input
 * 
 * @param map the current map
 * @param delta delta for the last frame draw
 */
void Player::tick(int **map, float delta) {
    double rot_speed = 2.5 * delta;
    double move_speed = 3 * delta;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if(map[int(this->x + this->dir_x * move_speed)][int(this->y)] == false) this->x += this->dir_x * move_speed;
        if(map[int(this->x)][int(this->y + this->dir_y * move_speed)] == false) this->y += this->dir_y * move_speed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if(map[int(this->x + this->dir_x * -move_speed)][int(this->y)] == false) this->x += this->dir_x * -move_speed;
        if(map[int(this->x)][int(this->y + this->dir_y * -move_speed)] == false) this->y += this->dir_y * -move_speed;
    }

    /* check for player input */
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        double old_dir = this->dir_x;
        double old_plane = this->plane_x;

        this->dir_x = this->dir_x * cos(rot_speed) - this->dir_y * sin(rot_speed);
        this->dir_y = old_dir * sin(rot_speed) + this->dir_y * cos(rot_speed);
        this->plane_x = this->plane_x * cos(rot_speed) - this->plane_y * sin(rot_speed);
        this->plane_y = old_plane * sin(rot_speed) + this->plane_y * cos(rot_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        double old_dir = this->dir_x;
        double old_plane = this->plane_x;

        this->dir_x = this->dir_x * cos(-rot_speed) - this->dir_y * sin(-rot_speed);
        this->dir_y = old_dir * sin(-rot_speed) + this->dir_y * cos(-rot_speed);
        this->plane_x = this->plane_x * cos(-rot_speed) - this->plane_y * sin(-rot_speed);
        this->plane_y = old_plane * sin(-rot_speed) + this->plane_y * cos(-rot_speed);
    }
}


/**
 * @brief Conducts the actual raycasting and DDA heavily based on:
 *        https://lodev.org/cgtutor/raycasting.html
 * 
 * @param window the active window to draw to
 * @param map the current map
 */
void Player::render(sf::RenderWindow &window, int **map) {
    /* loop through each x value of the screen */
    for (int x = 0; x < DISPL_WIDTH; ++x) {
        double camera_x = 2 * x / (double)DISPL_WIDTH - 1; /* project the x value onto the camera plane */
        double ray_dir_x = this->dir_x + this->plane_x * camera_x; /* project ray coordinates onto the camera space */
        double ray_dir_y = this->dir_y + this->plane_y * camera_x;

        int map_x = (int)this->x; /* get which map tile the ray is currently in */
        int map_y = (int)this->y; 

        double side_dist_x = 0; /* distance to the next side edge of the grid */
        double side_dist_y = 0;

        double delta_dist_x = (ray_dir_x == 0) ? 1e30 : std::abs(1 / ray_dir_x); /* get the distance needed to get to the next map edge */
        double delta_dist_y = (ray_dir_y == 0) ? 1e30 : std::abs(1 / ray_dir_y);
        double wall_dist; /* accumulator for total distance traveled */

        int step_x = ray_dir_x < 0 ? -1 : 1; /* what directtion to step in next, either +1, -1 */
        int step_y = ray_dir_y < 0 ? -1 : 1;

        bool hit = false; /* have we hit a wall */
        int side = 0;     /* used for shading */

        /* calculate distance to next cell border */
        if (ray_dir_x < 0) {
            side_dist_x = (this->x - map_x) * delta_dist_x;
        } else {
            side_dist_x = (map_x + 1.0 - this->x) * delta_dist_x;
        }

        if (ray_dir_y < 0) {
            side_dist_y = (this->y - map_y) * delta_dist_y;
        } else {
            side_dist_y = (map_y + 1.0 - this->y) * delta_dist_y;
        }

        /* perform DDA Algorithm */
        while (!hit) {
            if (side_dist_x < side_dist_y) { /* jump to closest map cell edge */
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            if (map[map_x][map_y] > 0) /* test for a hit */
                hit = true;
        }

        /* calculate ray distance */
        if (side == 0) { wall_dist = side_dist_x - delta_dist_x; }
        else           { wall_dist = side_dist_y - delta_dist_y; }

        /* now draw vertical stripe */
        int line_height = (int)(DISPL_HEIGHT / wall_dist);
        int draw_start = -line_height / 2 + DISPL_HEIGHT / 2;
        int draw_end = line_height / 2 + DISPL_HEIGHT / 2;

        if (draw_start < 0) draw_start = 0;
        if (draw_end >= DISPL_HEIGHT) draw_end = DISPL_HEIGHT - 1;

        sf::RectangleShape ver_line(sf::Vector2f(1 * DISPL_SCALE, (draw_start - draw_end) * DISPL_SCALE));
        ver_line.setPosition(sf::Vector2f(x * DISPL_SCALE, draw_end * DISPL_SCALE));

        sf::Color color = side == 1 ? sf::Color::Blue : sf::Color::Red;
        get_fog_adjusted_color(wall_dist, 5, color, sf::Color(0x323250));
        ver_line.setFillColor(color);

        if (color == sf::Color::Black) /* skip unnecessary draws */
            continue;

        window.draw(ver_line);
    }
}