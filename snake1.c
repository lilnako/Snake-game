#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define WIN_WIDTH 40
#define WIN_HEIGHT 20

enum Direction
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
};

int current_direction = DIR_RIGHT;

typedef struct position
{
    int y; ////////////change to float and make up dir increment by 0.5
    int x;

} position;

void start(position**, WINDOW**);
void input(position*, int, WINDOW*);
void new_enemy(position**, WINDOW*, bool*);
void game_logic(position*, position*, int*, bool*, int*);
void move_player(position* player);
void draw_screen(position*, position*, WINDOW*, bool);
void print_score(int);

int main()
{
    bool enemy_spawned = false;
    int score = 0; //keep track of the score
    int input_ch; //the input from getch()
    int speed = 70; //value for timeout, will decrease after catching the enemy
    position* player;
    position* enemy;
    WINDOW* game_win;
    start(&player, &game_win);
    move(0, 0);
    
    while (1)
    {
        timeout(speed);
        input_ch = getch();
        move(0, 0);
        print_score(score);
        move_player(player);
        new_enemy(&enemy, game_win, &enemy_spawned);
        input(player, input_ch, game_win);
        game_logic(player, enemy, &score, &enemy_spawned, &speed);
        draw_screen(player, enemy, game_win, enemy_spawned);
    }
    
    endwin();
    return 0;

    
}

void start(position** player, WINDOW** game_win)
{
    //various curses funcs on startup
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    refresh();

    //init player struct
    *player = (position*)malloc(sizeof(position));
    (*player)->x = WIN_WIDTH / 2;
    (*player)->y = WIN_HEIGHT / 2;
    
    //init game window
    *game_win = newwin(WIN_HEIGHT +1 , WIN_WIDTH + 1, 2, 2);
    box((*game_win), 0, 0);
    mvwaddch((*game_win), 10, 20, '#');
    // for (int i = 0; i <= WIN_HEIGHT; i+=5)
    // {
    //     for (int j = 0; j <= WIN_WIDTH; j+=5)
    //     {
    //         if (i == 10 && j == 20)
    //         {
    //             continue;
    //         }
    //         else
    //             mvwaddch((*game_win), i, j, '+');
    //     }
    // }
    
    wrefresh(*game_win); 
}

void input(position* player, int input_ch, WINDOW* game_win)
{
    switch (input_ch)
    {
        case 'a':
            current_direction = DIR_LEFT;
            break;
        case 'd':
            current_direction = DIR_RIGHT;
            break;
        case 's':
            current_direction = DIR_DOWN;
            break;
        case 'w':
            current_direction = DIR_UP;
            break;
    }
}

void new_enemy(position** enemy, WINDOW* game_win, bool* enemy_spawned)
{
    time_t t;
    srand((unsigned) time(&t));
    
    if (*enemy_spawned == false)
    {
        *enemy = (position*)malloc(sizeof(position));

        (*enemy)->x = (rand() % 39) + 1;
        (*enemy)->y = (rand() % 19) + 1;

        mvwaddch(game_win, (*enemy)->y, (*enemy)->x, '&');

        *enemy_spawned = true;
    }
}

void game_logic(position* player, position* enemy, int* score, bool* enemy_spawned, int* speed)
{
    if ((player->x == enemy->x) && (player->y == enemy->y))
    {
        *enemy_spawned = false;
        *score += 1;
        *speed -= 5;
    }
    
}
void move_player(position* player) //automatically move player according to current_direction
{ 
    switch (current_direction)
    {
    case DIR_RIGHT:
        if ((player->x + 1) != 40)
        {
            player->x++;
        }
        break;
    case DIR_LEFT:
        if ((player->x - 1) != 0)
        {
            player->x--;
        }
        break;
    case DIR_UP:
        if ((player->y - 1) != 0)
        {
            player->y--;
        }
        break;
    case DIR_DOWN:
        if ((player->y + 1) != 20)
        {
            player->y++;
        }
        break;
    }
}

void print_score(int score)
{
    printw("Score: %d        ", score);
}

void draw_screen(position* player, position* enemy, WINDOW* game_win, bool enemy_spawned)
{
    wclear(game_win);
    box(game_win, 0, 0);
    mvwaddch(game_win, enemy->y, enemy->x, '&');
    mvwaddch(game_win, player->y, player->x, '#');
    wrefresh(game_win);
}



