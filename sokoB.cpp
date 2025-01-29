#include "sokoB.h"

char ch;
int g_mode = 0;
bool fdebug = false;
int number = 1;
Level curLvl;

int main()
{
    startMenu();
    while (true)
    {
        ch = getch();
        readSignal(ch);
        if (fdebug)
            printf("\nyou pressed %c : %x\n", ch, ch);
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
    printf("\t2. load game\n");
    printf("\t3. debug\n");
    printf("\t[wasd] to move\n");
    printf("\t[space] to restart level\n");
    printf("\t[esc]/[ctrl+c] to exit\n");
    printf("note: when you exit game saves your current level\n");
}

void loadLevel()
{
    std::string reader;
    std::string name = "level" + std::to_string(number) + ".txt";
    Level rlvl;
    std::ifstream level(name);
    int ycall = 0;

    while (getline(level, reader))
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

    printf("%d/%d\n", number, LVLS);
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
    if (ch == 0x32)
    {
        std::ifstream load("dontedit.save", std::ios::binary);
        load >> number;
        load.close();
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
        if (number == 3)
        {
            printf("you have completed the main game!\n");
            startMenu();
            g_mode = 0;
        }
        else
        {
            number += 1;
            loadLevel();
        }
    }
}

void moveChar(char ch)
{
    if (ch == W_CHAR)
        change_cords(0, -1);
    if (ch == A_CHAR)
        change_cords(-1, 0);
    if (ch == S_CHAR)
        change_cords(0, 1);
    if (ch == D_CHAR)
        change_cords(1, 0);
}

void change_cords(int adder_x, int adder_y)
{
    vec2 move_to;

    move_to.point_x = curLvl.player_cords.point_x + adder_x;
    move_to.point_y = curLvl.player_cords.point_y + adder_y;

    if (is_movable(move_to, 'p'))
        swap(curLvl.player_cords, move_to, '@', 'p');
    else if (is_movable(move_to, 'b'))
    {
        vec2 move_box;

        move_box.point_x = move_to.point_x + adder_x;
        move_box.point_y = move_to.point_y + adder_y;

        if (is_movable(move_box, 'p'))
        {
            swap(move_to, move_box, 'b', 'b');
            swap(curLvl.player_cords, move_to, '@', 'p');
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
            if ((fpoints.at(i).point_x == boxes.at(j).point_x) &&
            (fpoints.at(i).point_y == boxes.at(j).point_y))
                copt++;
        }
    }

    printf("%d %zu\n", copt, fpoints.size());

    if (copt == fpoints.size())
        return true;
    return false;
}

bool is_movable(vec2 mv_to, char marker)
{
    char check_this = curLvl.lmap.at(mv_to.point_y).at(mv_to.point_x);
    if (marker == 'p')
    {
        if (check_this == ' ' || check_this == '.')
            return true;
        return false;
    }
    else if (marker == 'b')
    {
        if (check_this == 'b')
            return true;
        return false;
    }

    return false;
}

void swap(vec2 mv_from, vec2 mv_to, char swapper, char marker)
{
    if (find_cords(mv_from, curLvl.f_cords))
        curLvl.lmap.at(mv_from.point_y).at(mv_from.point_x) = DOTP;
    else
        curLvl.lmap.at(mv_from.point_y).at(mv_from.point_x) = 0x20;

    curLvl.lmap.at(mv_to.point_y).at(mv_to.point_x) = swapper;

    if (marker == 'p')
    {
        curLvl.player_cords.point_x = mv_to.point_x;
        curLvl.player_cords.point_y = mv_to.point_y;
    }
    else if (marker == 'b')
    {
        for (int i = 0; i < curLvl.box_cords.size(); i++)
        {
            if ((mv_from.point_y == curLvl.box_cords.at(i).point_y) &&
            (mv_from.point_x == curLvl.box_cords.at(i).point_x))
            {
                curLvl.box_cords.at(i).point_x = mv_to.point_x;
                curLvl.box_cords.at(i).point_y = mv_to.point_y;
            }
        }
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
        {
            std::ofstream save("dontedit.save", std::ios::binary);
            save << number << "\n";
            save.close();
            abort();
        }
        if (ask == 'n')
            break;
    }
}