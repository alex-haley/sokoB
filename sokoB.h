#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <ios>
#include <iostream>
#include <fstream>
#include <conio.h>

#define W_CHAR  0x77
#define A_CHAR  0x61
#define S_CHAR  0x73
#define D_CHAR  0x64
#define DOTP    0x2e
#define BOXP    0x62
#define PLRP    0x40
#define LVLS    4

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