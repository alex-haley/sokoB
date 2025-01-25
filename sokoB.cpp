#include <cstdio>
#include <string>
#include <vector>
#include <random>
#include <ios>
#include <iostream>
#include <fstream>
#include <conio.h>
#include "sokoB.h"

#define W_CHAR  0x77
#define A_CHAR  0x61
#define S_CHAR  0x73
#define D_CHAR  0x64
#define DOTP    0x2e
#define BOXP    0x62
#define PLRP    0x40

char ch;
int g_mode = 0;
bool fdebug = false;
Level curLvl;

int main()
{
    startMenu();
    while (true)
    {
        ch = getch();
        readSignal(ch);
        if (fdebug)
            debug(ch);
        if (g_mode == 1)
            renderLevel(curLvl);
    }

    return 0;
}

void startMenu()
{
    printf("\n");
    printf("mini-sokoban game made by alex-haley, 2025\n");
    printf("\t1. new game\n");
    printf("\t3. debug\n");
    printf("\t[space] to restart level\n");
    printf("\t[esc]/[ctrl+c] to exit\n");
}

void loadLevel()
{
    std::string reader;
    Level rlvl;
    std::ifstream level1("level1.txt");
    int ycall = 0;

    while (getline(level1, reader))
    {
        std::vector<char> lvlstring;
        vec2 bcords;
        vec2 fcords;

        for (int i = 0; i < reader.size(); i++)
        {
            if (reader.at(i) == PLRP)
            {
                rlvl.player_cords.point_x = i;
                rlvl.player_cords.point_y = ycall;
            }
            if (reader.at(i) == BOXP)
            {
                bcords.point_x = i;
                bcords.point_y = ycall;
                rlvl.box_cords.push_back(bcords);
            }
            if (reader.at(i) == DOTP)
            {
                fcords.point_x = i;
                fcords.point_y = ycall;
                rlvl.f_cords.push_back(fcords);
            }
            lvlstring.push_back(reader.at(i));
        }
        rlvl.lmap.push_back(lvlstring);
        ycall += 1;
    }

    printf("\nlevel loaded successfully.\n");
    printf("\nplayer: x: %d y: %d\n",
        rlvl.player_cords.point_x,
        rlvl.player_cords.point_y);

    for (int i = 0; i < rlvl.box_cords.size(); i++)
    {
        printf("box %d: x: %d, y: %d\n", i+1,
            rlvl.box_cords.at(i).point_x,
            rlvl.box_cords.at(i).point_y);
    }

    for (int i = 0; i < rlvl.f_cords.size(); i++)
    {
        printf("f-point %d: x: %d, y: %d\n", i+1,
            rlvl.f_cords.at(i).point_x,
            rlvl.f_cords.at(i).point_y);
    }

    curLvl = rlvl;
}

void renderLevel(Level blvl)
{
    for (int i = 0; i < blvl.lmap.size(); i++)
    {
        for (int j = 0; j < blvl.lmap.at(i).size(); j++)
        {
            printf("%c", blvl.lmap.at(i).at(j));
        }
        printf("\n");
    }
}

void readSignal(char ch)
{
    if (ch == 0x1b || ch == 0x3)
        ask_if_quit();
    if (ch == 0x31 && g_mode == 0)
    {
        g_mode = 1;
        loadLevel();
    }
    if (ch == 0x33)
    {
        if (fdebug)
            fdebug = false;
        else
            fdebug = true;
    }
    if ((ch == W_CHAR || ch == A_CHAR || ch == S_CHAR || ch == D_CHAR) && (g_mode == 1))
        moveChar(ch);
    if (ch == 0x20 && g_mode == 1)
        loadLevel();
    if (g_mode == 1 && check_end(curLvl.box_cords, curLvl.f_cords))
    {
        printf("you completed this level!\n");
        printf("\n");
        startMenu();
        g_mode = 0;
    }
}

void moveChar(char ch)
{
    if (ch == W_CHAR)
    {
        vec2 move_to;
        vec2 move_box;

        move_to.point_x = curLvl.player_cords.point_x;
        move_to.point_y = curLvl.player_cords.point_y-1;

        move_box.point_x = move_to.point_x;
        move_box.point_y = move_to.point_y-1;

        if (is_movable(move_to))
        {
            swap(curLvl.player_cords, move_to, '@');
            curLvl.player_cords.point_x = move_to.point_x;
            curLvl.player_cords.point_y = move_to.point_y;
        }
        else if (is_box(move_to))
        {
            if (is_movable(move_box))
            {
                swap(move_to, move_box, 'b');
                update_box_cords(move_to, move_box);
                swap(curLvl.player_cords, move_to, '@');
                curLvl.player_cords.point_x = move_to.point_x;
                curLvl.player_cords.point_y = move_to.point_y;
            }
        }
    }
    if (ch == A_CHAR)
    {
        vec2 move_to;
        vec2 move_box;

        move_to.point_x = curLvl.player_cords.point_x-1;
        move_to.point_y = curLvl.player_cords.point_y;

        move_box.point_x = move_to.point_x-1;
        move_box.point_y = move_to.point_y;

        if (is_movable(move_to))
        {
            swap(curLvl.player_cords, move_to, '@');
            curLvl.player_cords.point_x = move_to.point_x;
            curLvl.player_cords.point_y = move_to.point_y;
        }
        else if (is_box(move_to))
        {
            if (is_movable(move_box))
            {
                swap(move_to, move_box, 'b');
                update_box_cords(move_to, move_box);
                swap(curLvl.player_cords, move_to, '@');
                curLvl.player_cords.point_x = move_to.point_x;
                curLvl.player_cords.point_y = move_to.point_y;
            }
        }
    }
    if (ch == S_CHAR)
    {
        vec2 move_to;
        vec2 move_box;

        move_to.point_x = curLvl.player_cords.point_x;
        move_to.point_y = curLvl.player_cords.point_y+1;

        move_box.point_x = move_to.point_x;
        move_box.point_y = move_to.point_y+1;

        if (is_movable(move_to))
        {
            swap(curLvl.player_cords, move_to, '@');
            curLvl.player_cords.point_x = move_to.point_x;
            curLvl.player_cords.point_y = move_to.point_y;
        }
        else if (is_box(move_to))
        {
            if (is_movable(move_box))
            {
                swap(move_to, move_box, 'b');
                update_box_cords(move_to, move_box);
                swap(curLvl.player_cords, move_to, '@');
                curLvl.player_cords.point_x = move_to.point_x;
                curLvl.player_cords.point_y = move_to.point_y;
            }
        }
    }
    if (ch == D_CHAR)
    {
        vec2 move_to;
        vec2 move_box;

        move_to.point_x = curLvl.player_cords.point_x+1;
        move_to.point_y = curLvl.player_cords.point_y;

        move_box.point_x = move_to.point_x+1;
        move_box.point_y = move_to.point_y;

        if (is_movable(move_to))
        {
            swap(curLvl.player_cords, move_to, '@');
            curLvl.player_cords.point_x = move_to.point_x;
            curLvl.player_cords.point_y = move_to.point_y;
        }
        else if (is_box(move_to))
        {
            if (is_movable(move_box))
            {
                swap(move_to, move_box, 'b');
                update_box_cords(move_to, move_box);
                swap(curLvl.player_cords, move_to, '@');
                curLvl.player_cords.point_x = move_to.point_x;
                curLvl.player_cords.point_y = move_to.point_y;
            }
        }
    }
}

bool check_end(std::vector<vec2> boxes, std::vector<vec2> fpoints)
{
    int copt = 0;
    for (int i = 0; i < fpoints.size(); i++)
    {
        for (int j = 0; j < boxes.size(); j++)
        {
            if ((fpoints.at(i).point_x == boxes.at(j).point_x) && (fpoints.at(i).point_y == boxes.at(j).point_y))
                copt++;
        }
    }

    printf("%d %zu\n", copt, fpoints.size());

    if (copt == fpoints.size())
        return true;
    return false;
}

bool is_movable(vec2 mv_to)
{
    char check_this = curLvl.lmap.at(mv_to.point_y).at(mv_to.point_x);
    if (check_this == ' ' || check_this == '.')
        return true;
    return false;
}

bool is_box(vec2 bc)
{
    if (curLvl.lmap.at(bc.point_y).at(bc.point_x) == 'b')
        return true;
    return false;
}

void swap(vec2 mv_from, vec2 mv_to, char swapper)
{
    if (find_cords(mv_from, curLvl.f_cords))
    {
        curLvl.lmap.at(mv_from.point_y).at(mv_from.point_x) = DOTP;
        curLvl.lmap.at(mv_to.point_y).at(mv_to.point_x) = swapper;
    }
    else
    {
        curLvl.lmap.at(mv_from.point_y).at(mv_from.point_x) = 0x20;
        curLvl.lmap.at(mv_to.point_y).at(mv_to.point_x) = swapper;
    }
}

bool find_cords(vec2 ft, std::vector<vec2> here)
{
    for (int i = 0; i < here.size(); i++)
    {
        if ((ft.point_y == here.at(i).point_y) && (ft.point_x == here.at(i).point_x))
        {
            return true;
        }
    }
    return false;
}

void update_box_cords(vec2 old_box, vec2 new_box)
{
    for (int i = 0; i < curLvl.box_cords.size(); i++)
    {
        if ((old_box.point_y == curLvl.box_cords.at(i).point_y) && (old_box.point_x == curLvl.box_cords.at(i).point_x))
        {
            curLvl.box_cords.at(i).point_x = new_box.point_x;
            curLvl.box_cords.at(i).point_y = new_box.point_y;
        }
    }
}

void debug(char ch)
{
    printf("\nyou pressed %c : %x\n", ch, ch);
}

void ask_if_quit()
{
    char ask;
    while (true)
    {
        printf("\ndo you really wonna exit this beautiful game??\n");
        printf("(y/n): ");
        ask = getch();
        printf("\n");
        if (ask == 'y')
            abort();
        if (ask == 'n')
            break;
    }
}