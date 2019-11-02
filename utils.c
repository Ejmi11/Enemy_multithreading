#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "iaik_rq.h"
#include <string.h>


void* createHandOverParameters(parameters* params)
{
    void* parameter;
    memcpy(&parameter, params, 6);
    return parameter;
}

parameters parseHandOverParameters(void* params)
{
    parameters parameter;
    memcpy(&parameter, &params, 6);
    return parameter;
}

int randomEnemySpawnPos()
{
  unsigned int rand_num = time(NULL);
  return rand_r (&rand_num)%(BOARD_WIDTH - 4);
}
int randomEnemyMovement()
{
 unsigned int rand_num = time(NULL);
 return rand_r(&rand_num)%3;
}
int randomEnemyType()
{
  unsigned int rand_num = time(NULL);
  return rand_r(&rand_num)%3;
  
}
position randomUpgradePosition()
{
  position pos = {0,0};
  unsigned int rand_num = time(NULL);
  pos.y_ = (rand_r(&rand_num)%(BOARD_LENGTH/2)) + (BOARD_LENGTH/2);
  pos.x_ = rand_r(&rand_num)%(BOARD_WIDTH);
  return pos;
}

void initBoard()
{

  //only change this when needed and revert this!

  life = 5;
  curr_enemies = AMOUNT_ENEMIES;


  for(int y = 0; y < BOARD_LENGTH; y++)
  {
    for(int x = 0; x < BOARD_WIDTH; x++)
    {
      board[y][x] = (char)EMPTY_SPACE;
    }
  }

  //enable curses
  initscr();


  game_window = newwin(BOARD_LENGTH, BOARD_WIDTH, 0, 0);
  printBoard();

}

void gameOver()
{

  //Preparing Game-Over Board!
  for(int y = 3; y < BOARD_LENGTH; y++)
  {
    for(int x = 0; x < BOARD_WIDTH; x++)
    {
      board[y][x] = ENEMY_ID_1;
      printBoard();
      usleep(2);
    }
  }


  mvwprintw(game_window, (BOARD_LENGTH/2) ,16, "   GAME OVER   ");
  wrefresh(game_window);
  usleep(5000000);
  wborder(game_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(game_window);
  delwin(game_window);
  endwin();
}

