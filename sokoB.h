#pragma once

struct vec2
{
    int point_x;
    int point_y;
};

struct Level
{
    std::vector<std::vector<char>> lmap;
    vec2 player_cords;
    std::vector<vec2> box_cords;
    std::vector<vec2> f_cords;
};

bool is_movable(vec2, char);
bool find_cords(vec2, std::vector<vec2>);
bool check_end(std::vector<vec2>, std::vector<vec2>);
void change_cords(int, int);
void swap(vec2, vec2, char, char);
void startMenu();
void loadLevel();
void renderLevel(Level);
void readSignal(char);
void moveChar(char);
void debug(char);
void ask_if_quit();