#include "sokoB.h"

// TODO: test inline functions. if they would be faster and occupy less memory, we can use them.

char ch;
u8 g_mode = 0;
bool fdebug = false;
u8 number = 0;
u8 how_many_boxes = 0;
Level curLvl;
s8 X = 0;
s8 Y = 0;

int main()
{
    startMenu();
    while (true)
    {
        ch = _getch();
        read_signal(ch);
        if (fdebug)
            printf("\nyou pressed %c : %x\n", ch, ch);
        if (g_mode != 0)
            render_level(curLvl);
    }

    return 0;
}

void startMenu()
{
    printf("\n");
    printf("mini-sokoban game made by alex-haley, 2025\n\n");
    printf("\t1. start game\n");
    printf("\t2. load level from file\n");
    printf("\t3. debug\n\n");
    printf("\tcontrols:\n\n");
    printf("\t[WASD] move\n");
    printf("\t[Q] undo latest move\n");
    printf("\t[SPACE] restart level\n");
    printf("\t[ESC] exit\n\n");
    printf("NOTE: when you exit game saves your current level\n");
    printf("\tyou can delete save file to start a fresh game\n");
    printf("NOTE: when loading level from file it should be named as 'level1.txt'\n");
}

void load_map()
{
    std::vector<std::vector<char>> const map_table[LVLS] = {lvl1, lvl2, lvl3, lvl4};
    Level rlvl;

    if (g_mode == 2) {rlvl.lmap = load_from_file();}
    else             {rlvl.lmap = map_table[number];}

    vec2 bcords;
    vec2 fcords;
    for (int i = 0; i < rlvl.lmap.size(); i++)
    {
        for (int j = 0; j < rlvl.lmap.at(i).size(); j++)
        {
            if (rlvl.lmap.at(i).at(j) == PLRP)
            {
                rlvl.player_cords.point_x = j;
                rlvl.player_cords.point_y = i;
            }
            if (rlvl.lmap.at(i).at(j) == BOXP)
            {
                bcords.point_x = j;
                bcords.point_y = i;
                rlvl.box_cords.push_back(bcords);
            }
            if (rlvl.lmap.at(i).at(j) == DOTP)
            {
                fcords.point_x = j;
                fcords.point_y = i;
                rlvl.f_cords.push_back(fcords);
            }
        }
    }

    curLvl = rlvl;
    how_many_boxes = 0;

    printf("\n[m]level loaded successfully.\n");
    printf("\nplayer: \tx: %d \ty: %d\n",
        curLvl.player_cords.point_x,
        curLvl.player_cords.point_y);

    for (int i = 0; i < curLvl.box_cords.size(); i++)
    {
        printf("box%d: \t\tx: %d \ty: %d\n", i+1,
            curLvl.box_cords.at(i).point_x,
            curLvl.box_cords.at(i).point_y);
    }

    for (int i = 0; i < curLvl.f_cords.size(); i++)
    {
        printf("f-point%d: \tx: %d \ty: %d\n", i+1,
            curLvl.f_cords.at(i).point_x,
            curLvl.f_cords.at(i).point_y);
    }
}

std::vector<std::vector<char>> load_from_file()
{
    std::vector<std::vector<char>> lvlmat;
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

void render_level(Level blvl)
{
    for (int i = 0; i < blvl.lmap.size(); i++)
    {
        for (int j = 0; j < blvl.lmap.at(i).size(); j++)
        {
            printf("%c", blvl.lmap.at(i).at(j));
        }
        printf("\n");
    }

    printf("%d/%zu\n", how_many_boxes, curLvl.f_cords.size());
}

void read_signal(char ch)
{
    switch (ch)
    {
        case ESC:
            printf("\npress [esc] again\n");
            char ask;
            ask = _getch();
            printf("\n");

            switch (ask)
            {
                case CTRL_C: break;

                case ESC:
                    std::ofstream save("dontedit.save", std::ios::binary);
                    save << number << "\n";
                    save.close();
                    exit(0);
                    break;
            } break;

        case ONE:
            if (g_mode == 0)
            {
                std::ifstream load("dontedit.save", std::ios::binary);
                load >> number;
                load.close();
                g_mode = 1;
                load_map();
            } break;

        case TWO:
            if (g_mode == 0)
            {
                g_mode = 2;
                load_map();
            } break;

        case THREE:
            if (fdebug)
            {
                fdebug = false;
                startMenu();
            }
            else
                fdebug = true;
            break;

        case W:
            if (g_mode != 0)
            {
                change_cords(0,-1, 0);
                X = 0;
                Y = -1;
                break;
            }

        case A:
            if (g_mode != 0)
            {
                change_cords(-1,0, 0);
                X = -1;
                Y = 0;
                break;
            }

        case S:
            if (g_mode != 0)
            {
                change_cords(0,1, 0);
                X = 0;
                Y = 1;
                break;
            }

        case D:
            if (g_mode != 0)
            {
                change_cords(1,0, 0);
                X = 1;
                Y = 0;
                break;
            }

        case Q:
            if (g_mode != 0)
            {
                change_cords(X, Y, 1);
                X = 0;
                Y = 0;
                break;
            }

        case SPACE:
            if (g_mode == 1)
                load_map();
            if (g_mode == 2)
                load_map();

        default: {};
    }

    if (check_end(curLvl.box_cords, curLvl.f_cords))
    {
        if (g_mode == 2)
        {
            printf("you have completed this level!\n");
            printf("\n");
            startMenu();
            g_mode = 0;
        }
        if (g_mode == 1)
        {
            number++;
            load_map();
        }
    }
}

s8 invert(s8 d)
{
    if (d != 0)
        return (d ^ 0xFF)|0x1;
    else
        return d;
}

void change_cords(s8 adder_x, s8 adder_y, char mark)
{
    vec2 move_to;

    move_to.point_x = curLvl.player_cords.point_x + adder_x;
    move_to.point_y = curLvl.player_cords.point_y + adder_y;

    if (is_movable(move_to, 'p'))
    {
        if (mark == 1)
        {
            move_to.point_x = curLvl.player_cords.point_x + invert(adder_x);
            move_to.point_y = curLvl.player_cords.point_y + invert(adder_y);
        }
        swap(curLvl.player_cords, move_to, '@', 'p');
    }

    else if (is_movable(move_to, 'b'))
    {
        vec2 move_box;

        move_box.point_x = move_to.point_x + adder_x;
        move_box.point_y = move_to.point_y + adder_y;
        if (mark == 1)
        {
            vec2 inm;
            vec2 dnm;

            move_to.point_x = curLvl.player_cords.point_x + invert(adder_x);
            move_to.point_y = curLvl.player_cords.point_y + invert(adder_y);
            swap(curLvl.player_cords, move_to, '@', 'p');

            inm.point_x = move_to.point_x + adder_x;
            inm.point_y = move_to.point_y + adder_y;
            dnm.point_x = inm.point_x + adder_x;
            dnm.point_y = inm.point_y + adder_y;
            swap(dnm, inm, 'b', 'b');
        }
        else
        {
             if (is_movable(move_box, 'p'))
            {
                swap(move_to, move_box, 'b', 'b');
                swap(curLvl.player_cords, move_to, '@', 'p');
            }
        }
    }
}

bool check_end(std::vector<vec2> boxes, std::vector<vec2> fpoints)
{
    u8 copt = 0;
    for (int i = 0; i < fpoints.size(); i++)
    {
        for (int j = 0; j < boxes.size(); j++)
        {
            if ((fpoints.at(i).point_x == boxes.at(j).point_x) &&
            (fpoints.at(i).point_y == boxes.at(j).point_y))
                copt++;
        }
    }
    how_many_boxes = copt;

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