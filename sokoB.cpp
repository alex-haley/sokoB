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
        if (g_mode == 1 || g_mode == 2)
            renderLevel(curLvl);
    }

    return 0;
}

void startMenu()
{
    printf("\n");
    printf("mini-sokoban game made by alex-haley, 2025\n");
    printf("\t1. start game\n");
    printf("\t2. load level from file\n");
    printf("\t3. debug\n");
    printf("\t[wasd] to move\n");
    printf("\t[space] to restart level\n");
    printf("\t[esc]/[ctrl+c] to exit\n");
    printf("NOTE: when you exit game saves your current level\n");
    printf("\tyou can delete save file to start a fresh game\n");
    printf("NOTE: when loading level from file it should be named as 'level1.txt'\n");
}

std::vector<std::vector<char>> pick_map()
{
    switch (number)
    {
        case 1:
            return lvl1;
            break;
        case 2:
            return lvl2;
            break;
        case 3:
            return lvl3;
            break;
        case 4:
            return lvl4;
            break;
        default:
            printf("you have finished the game!\n");
            exit(-1);
            break;
    }
}

void load_map(std::vector< std::vector<char> > load_this_map)
{
    Level rlvl;
    rlvl.lmap = load_this_map;
    vec2 bcords;
    vec2 fcords;
    for (int i = 0; i < load_this_map.size(); i++)
    {
        for (int j = 0; j < load_this_map.at(i).size(); j++)
        {
            if (load_this_map.at(i).at(j) == PLRP)
            {
                rlvl.player_cords.point_x = j;
                rlvl.player_cords.point_y = i;
            }
            if (load_this_map.at(i).at(j) == BOXP)
            {
                bcords.point_x = j;
                bcords.point_y = i;
                rlvl.box_cords.push_back(bcords);
            }
            if (load_this_map.at(i).at(j) == DOTP)
            {
                fcords.point_x = j;
                fcords.point_y = i;
                rlvl.f_cords.push_back(fcords);
            }
        }
    }

    curLvl = rlvl;

    printf("\n[m]level loaded successfully.\n");
    printf("\nplayer: x: %d y: %d\n",
        curLvl.player_cords.point_x,
        curLvl.player_cords.point_y);

    for (int i = 0; i < curLvl.box_cords.size(); i++)
    {
        printf("box %d: x: %d, y: %d\n", i+1,
            curLvl.box_cords.at(i).point_x,
            curLvl.box_cords.at(i).point_y);
    }

    for (int i = 0; i < curLvl.f_cords.size(); i++)
    {
        printf("f-point %d: x: %d, y: %d\n", i+1,
            curLvl.f_cords.at(i).point_x,
            curLvl.f_cords.at(i).point_y);
    }
}

std::vector< std::vector<char> > load_from_file()
{
    std::vector< std::vector<char> > lvlmat;
    std::string reader;
    std::string name = "level1.txt";
    std::ifstream level(name);

    while (getline(level, reader))
    {
        std::vector<char> lvlstring;

        for (int i = 0; i < reader.size(); i++)
        {
            lvlstring.push_back(reader.at(i));
        }

        lvlmat.push_back(lvlstring);
    }

    return lvlmat;
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
        std::ifstream load("dontedit.save", std::ios::binary);
        load >> number;
        load.close();
        g_mode = 1;
        load_map(pick_map());
    }
    if (ch == 0x32 && g_mode == 0)
    {
        g_mode = 2;
        load_map(load_from_file());
    }
    if (ch == 0x33)
    {
        if (fdebug)
            fdebug = false;
        else
            fdebug = true;
    }
    if ((ch == W_CHAR || ch == A_CHAR || ch == S_CHAR || ch == D_CHAR) && (g_mode != 0))
        moveChar(ch);
    if (ch == 0x20 && g_mode == 1)
        load_map(pick_map());
    if (ch == 0x20 && g_mode == 2)
        load_map(load_from_file());
    if ((g_mode != 0) && check_end(curLvl.box_cords, curLvl.f_cords))
    {
        if (g_mode == 2)
        {
            printf("you have completed this level!\n");
            printf("\n");
            startMenu();
            g_mode = 0;
        }
        else if (g_mode == 1)
        {
            number++;
            load_map(pick_map());
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
            exit(0);
        }
        if (ask == 'n')
            break;
    }
}