#include <ncurses.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <time.h> 
#include <stdlib.h> 
#include "iaik_rq.h"

int points = 0;
int life = 0;
int power_up_shots = 0;
int missed = 0;
int curr_enemies = 0;
char board[BOARD_LENGTH][BOARD_WIDTH];
pthread_t own_ship_tid;
position your_ship = {
  BOARD_WIDTH / 2,
  BOARD_LENGTH - 2
};
WINDOW *game_window;
char ** args;


char my_ship[4] = {'U','U', 'U', '\0'};
char enemy[3][6] = {{'O','O','O','\0'},
                    {'O','O','O','O','\0'},
                    {'O','O','O','O','O','\0'}};

void pointsManager(int type)
{
  //TODO Make sure to assign the right values to your points!!!
  //Look at the #defines provided!
    if (type == OWN_SHIP)
    {points = points + POINTS_SELF_DESTR; }
    else if (type == ENEMY_SHOT)
    {points = points + POINTS_SHOT; }
    else if (type == ENEMY_ID_1)
    {points = points + POINTS_SMALL_SHIP; }
    else if (type == ENEMY_ID_2)
    {points = points + POINTS_MED_SHIP; }
    else if (type == ENEMY_ID_3)
    {points = points + POINTS_BIG_SHIP; }
}

void printBoard()
{
  werase(game_window);
  for (int y = 0; y < BOARD_LENGTH; ++y)
  {
    for (int x = 0; x < BOARD_WIDTH; ++x)
    {
      if (board[y][x] != EMPTY_SPACE)
      {
        if (board[y][x] <= ENEMY_ID_3)
          mvwprintw(game_window, y, x, enemy[(short) board[y][x]]);
        else if (board[y][x] == OWN_SHIP)
          mvwprintw(game_window, y, x, my_ship);
        else if (board[y][x] == ENEMY_SHOT)
          mvwprintw(game_window, y, x, "v");
        else if (board[y][x] == OWN_SHOT)
          mvwprintw(game_window, y, x, ".");
        else if (board[y][x] == UPGRADE)
          mvwprintw(game_window, y, x, "X");
      }
    }
  }
  mvwprintw(game_window, 1, 1, "IAIK-RAGEQUITTER");
  mvwprintw(game_window, 1, 43, "v.1.2.0");
  mvwprintw(game_window, 2, 34, "Points: %08d", points);
  mvwprintw(game_window, 2, 11, "Missed: %d", missed);
  mvwprintw(game_window, 2, 1, "Life: %d", life);
  mvwprintw(game_window, 2, 22, "Shots:%04d", power_up_shots);

  box(game_window, 0, 0);
  wrefresh(game_window);
}

void launchShots(size_t type, char x, char y)
{
  /*TODO
   Do your magic here, you have to implement
   the whole function for this.
   This includes launching shots and handling
   all params needed for those*/

    if (type == OWN_SHIP_REGULAR_SHOT)
    {
        pthread_t shot1;

        position posit;
        posit.x_ = your_ship.x_ + 1;
        posit.y_ = your_ship.y_ - 1;
        type = SHOT_TYPE_OWN;
        parameters param = {posit, type};
     
        pthread_create(&shot1, NULL, shot, createHandOverParameters(&param));
    }
    else if (type == OWN_SHIP_UPGRADE_SHOT)
    {
        pthread_t shot2;
        pthread_t shot3;

        position posit1;
        position posit2;
        posit1.x_ = your_ship.x_;
        posit1.y_ = your_ship.y_ - 1;
        posit2.x_ = your_ship.x_ + 2;
        posit2.y_ = your_ship.y_ - 1;
        type = SHOT_TYPE_OWN;
        parameters param1 = {posit1, type};
        parameters param2 = {posit2, type};
        
        pthread_create(&shot2, NULL, shot, createHandOverParameters(&param1));
        pthread_create(&shot3, NULL, shot, createHandOverParameters(&param2));
    }
    else if (type == ENEMY_ID_1_SHOTS)
    {
        pthread_t shot4;

        position posit3;
        posit3.x_ = x + 1;
        posit3.y_ = y + 2;

        type = SHOT_TYPE_ENEMY;
        parameters param3 = {posit3, type};
                
        pthread_create(&shot4, 0, shot, createHandOverParameters(&param3));
    }
    else if (type == ENEMY_ID_2_SHOTS)
    {
        pthread_t shot5;
        pthread_t shot6;

        position posit4;
        position posit5;

        posit4.x_ = x;
        posit4.y_ = y + 2;
        posit5.x_ = x + 3;
        posit5.y_ = y + 2;

        type = SHOT_TYPE_ENEMY;
        parameters param4 = {posit4, type};
        parameters param5 = {posit5, type};
        
        pthread_create(&shot5, 0, shot, createHandOverParameters(&param4));
        pthread_create(&shot6, 0, shot, createHandOverParameters(&param5));

    }
    else if (type == ENEMY_ID_3_SHOTS)
    {
        pthread_t shot7;
        pthread_t shot8;
        pthread_t shot9;

        position posit6;
        position posit7;
        position posit8;

        posit6.x_ = x;
        posit6.y_ = y + 2;
        posit7.x_ = x + 2;
        posit7.y_ = y + 2;
        posit8.x_ = x + 4;
        posit8.y_ = y + 2;

        type = SHOT_TYPE_ENEMY;
        parameters param6 = {posit6, type};
        parameters param7 = {posit7, type};
        parameters param8 = {posit8, type};
        
        pthread_create(&shot7, 0, shot, createHandOverParameters(&param6));
        pthread_create(&shot8, 0, shot, createHandOverParameters(&param7));
        pthread_create(&shot9, 0, shot, createHandOverParameters(&param8));
    }
}

void *ship()
{
  int ch;
  board[your_ship.y_][your_ship.x_] = (char) OWN_SHIP;

  /*Do not touch the pthread_setcanceltype
   *function! You should not use this function
   *in any other thread created!
   */
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  while (0xcafebabe)
  {
    //you'll need this variable urgently!!
    ch = getchar();

    if ((ch == 'a' || ch == 'A') && (your_ship.x_ - 1) > 0)
    {
      board[your_ship.y_][your_ship.x_] = (char) EMPTY_SPACE;
      board[your_ship.y_][--your_ship.x_] = (char) OWN_SHIP;
    }
    else if ((ch == 'w' || ch == 'W') && (your_ship.y_ - 1) > 2)
    {
      board[your_ship.y_][your_ship.x_] = (char) EMPTY_SPACE;
      board[--your_ship.y_][your_ship.x_] = (char) OWN_SHIP;
    }
    else if ((ch == 's'|| ch == 'S') && (your_ship.y_ + 1) < (BOARD_LENGTH - 1))
    {
      board[your_ship.y_][your_ship.x_] = (char) EMPTY_SPACE;
      board[++your_ship.y_][your_ship.x_] = (char) OWN_SHIP;
    }
    else if ((ch == 'd' || ch == 'D') && (your_ship.x_ + 2) < (BOARD_WIDTH - 2))
    {
      board[your_ship.y_][your_ship.x_] = (char) EMPTY_SPACE;
      board[your_ship.y_][++your_ship.x_] = (char) OWN_SHIP;
    }
    else if (ch == ' ')
    {
      if (board[your_ship.y_ - 1][your_ship.x_] == OWN_SHOT ||
        board[your_ship.y_ - 1][your_ship.x_ + 1] == OWN_SHOT ||
        board[your_ship.y_ - 1][your_ship.x_ + 2] == OWN_SHOT)
        continue;


      if ((power_up_shots) > 0)
      {
        power_up_shots--;
        launchShots(OWN_SHIP_UPGRADE_SHOT, your_ship.x_, your_ship.y_);
      }
      else
      {
        launchShots(OWN_SHIP_REGULAR_SHOT, your_ship.x_, your_ship.y_);
      }
    }
    else if (ch == 'q')
    {
      life = 0;
    }
    ch = 0;
  }
  //not needed!
  return RETURN_VALUE_SHIP;
}

void *shot(void *params)
{
  /*TODO begin
   *This get your params here, there is also a function
   *for that! Look carefully, how you get them and what
   *how you are assigning those*/
  
  short x = 0;
  short y = 0;
  size_t type = 0;

  parameters temp_params = parseHandOverParameters(params);

  x = temp_params.pos_.x_;
  y = temp_params.pos_.y_;
  type = temp_params.type_;


  int time_count = 0;

  usleep(USLEEP_DEFAULT);

  if (!type)
    board[y][x] = (char) ENEMY_SHOT;
  else
    board[y][x] = (char) OWN_SHOT;

  do {

    usleep(900);
    if ((++time_count) == 190)
    {
      if (board[y][x] == EMPTY_SPACE)
      {
        if (type)
          pointsManager(ENEMY_SHOT);
          
        return 0;
      }

      board[y][x] = EMPTY_SPACE;

      if (!type)
      {
        if (board[y + 1][x] == OWN_SHOT)
        {
          board[y + 1][x] = (char) EMPTY_SPACE;
          return 0;
        }

        if (((x) > 0 && board[y + 1][x] <= OWN_SHIP) ||
          ((x - 1) > 0 && board[y + 1][x - 1] <= OWN_SHIP) ||
          ((x - 2) > 0 && board[y + 1][x - 2] <= OWN_SHIP) ||
          ((x - 3) > 0 && board[y + 1][x - 3] == ENEMY_ID_2) ||
          ((x - 4) > 0 && board[y + 1][x - 4] == ENEMY_ID_3))
        {
          if ((board[y + 1][x - 2] == OWN_SHIP) || (board[y + 1][x - 1] == OWN_SHIP) || (board[y + 1][x] == OWN_SHIP))
            life--;

          return 0;
        }
        //for better collission detection
        if (((x) > 0 && board[y][x] <= OWN_SHIP) ||
          ((x - 1) > 0 && board[y][x - 1] <= OWN_SHIP) ||
          ((x - 2) > 0 && board[y][x - 2] <= OWN_SHIP) ||
          ((x - 3) > 0 && board[y][x - 3] == ENEMY_ID_2) ||
          ((x - 4) > 0 && board[y][x - 4] == ENEMY_ID_3))
        {
          if ((board[y][x - 2] == OWN_SHIP) || (board[y][x - 1] == OWN_SHIP) || (board[y][x] == OWN_SHIP))
            life--;

          return 0;
        }
        board[++y][x] = ENEMY_SHOT;
      }
      else if (type)
      {
        //Shot collision detection
        if (board[y - 1][x] == ENEMY_SHOT)
        {
          board[y - 1][x] = (char) EMPTY_SPACE;
          pointsManager(ENEMY_SHOT);
          return 0;
        }

        for (int offset_y = 1; offset_y <= 2; offset_y++)
        {
          for (int offset_x = 0; offset_x <= 4; offset_x++)
          {
            if (offset_x <= 2)
            {
              if ((offset_x) >= 0 && board[y - offset_y][x - offset_x] <= OWN_SHIP)
              {
                if (board[y - offset_y][x - offset_x] == (char) OWN_SHIP)
                  life--;

                pointsManager(board[y - offset_y][x - offset_x]);
                board[y - offset_y][x - offset_x] = (char) EMPTY_SPACE;
                return 0;
              }
            }
            else
            {
              if ((x - offset_x) > 0 && offset_x == 3 && (board[y - offset_y][x - offset_x] == (char) ENEMY_ID_3 ||
                  board[y - offset_y][x - offset_x] == (char) ENEMY_ID_2))
              {
                pointsManager(board[y - offset_y][x - offset_x]);
                board[y - offset_y][x - offset_x] = (char) EMPTY_SPACE;
                return 0;
              }
              else if ((x - offset_x) > 0 && (board[y - offset_y][x - offset_x] == (char) ENEMY_ID_3))
              {
                pointsManager(board[y - offset_y][x - offset_x]);
                board[y - offset_y][x - offset_x] = (char) EMPTY_SPACE;
                return 0;
              }
            }
          }
        }
        board[y][x] = EMPTY_SPACE;
        board[--y][x] = OWN_SHOT;
      }

      time_count = 0;
    }

    if (y <= 2 || y >= (BOARD_LENGTH - 1))
    {
      board[y][x] = EMPTY_SPACE;
      return 0;
    }
  } while (life > 0);

  return 0;
}
void *enemies()
{
  //needed variables
  int time_count = 0;
  int random_num = 0;
  //Mod 3 for 3 types of enemies
  int type_of_enemy = randomEnemyType();

  int x = randomEnemySpawnPos();
  short y = 3;

  //x-coordinate + (length - 1) enemy and remaining left border (1)
  if (x == 0)
    x++;

  board[y][x] = type_of_enemy;

  do {
    usleep(2000);

    time_count++;

    if ((board[y][x] == EMPTY_SPACE || (board[y][x] == OWN_SHOT)))
    {
      curr_enemies++;
      return 0;
    }

    //Collission-detection for your ship against enemies
    if ((board[y][x] == OWN_SHIP ||
        board[y][x - 1] == OWN_SHIP ||
        board[y][x - 2] == OWN_SHIP ||
        board[y][x + 1] == OWN_SHIP ||
        board[y][x + 2] == OWN_SHIP) ||
      (type_of_enemy == ENEMY_ID_2 && board[y][x + 3] == OWN_SHIP) ||
      (type_of_enemy == ENEMY_ID_3 && (board[y][x + 3] == OWN_SHIP || board[y][x + 4] == OWN_SHIP)))
    {
      life = 0;
    }

    if ((time_count) % 500 == 0)
    {
      random_num = randomEnemyMovement();
      if (random_num == 0 && ((x - 1) > 1))
      {
        board[y][x] = (char) EMPTY_SPACE;
        x--;
        board[y][x] = (char) type_of_enemy;
      }
      else if (random_num == 1 && ((x + type_of_enemy + 3) < (BOARD_WIDTH - 1)))
      {
        board[y][x] = (char) EMPTY_SPACE;
        x++;
        board[y][x] = (char) type_of_enemy;
      }
    }
    if ((time_count) % SHOOTING_SPEED == 0)
    {
      if (type_of_enemy == ENEMY_ID_1)
      {
        launchShots(ENEMY_ID_1_SHOTS, x, y);
      }
      else if (type_of_enemy == ENEMY_ID_2)
      {
        launchShots(ENEMY_ID_2_SHOTS, x, y);

      }
      else if (type_of_enemy == ENEMY_ID_3)
      {
        launchShots(ENEMY_ID_3_SHOTS, x, y);

      }
      time_count = 0;
    }

    if ((time_count) % 300 == 0)
    {
      if (y == (BOARD_LENGTH - 1))
      {
        missed++;
        return 0;
      }
      board[y][x] = (char) EMPTY_SPACE;
      y++;
      board[y][x] = (char) type_of_enemy;
    }

  } while (life > 0);

  return 0;
}

void *upgradePlacer()
{
  position pos = randomUpgradePosition();
  int y = pos.y_;
  int x = pos.x_;
  board[y][x] = UPGRADE;
  size_t count = 0;
  do {
    usleep(USLEEP_DEFAULT * 2);
    count++;

    if (board[y][x] == OWN_SHIP || board[y][x - 1] == OWN_SHIP || board[y][x - 2] == OWN_SHIP)
    {
      //TODO modify your powerup shots!

      power_up_shots = power_up_shots + POWER_UP_SHOTS;

      if (board[y][x] == OWN_SHIP)
        board[y][x] = OWN_SHIP;
      else
        board[y][x] = EMPTY_SPACE;
      return 0;
    }
    else if (board[y][x] != UPGRADE)
    {
      board[y][x] = EMPTY_SPACE;
      return 0;
    }
    else if ((count % 10000) == 0)
    {
      board[y][x] = (char) EMPTY_SPACE;
      return 0;
    }

  } while (life > 0);

  return 0;
}

int main(int argc, char *argv[])
{
  //DO NOT REMOVE OR MODIFY THIS!!!
  args = argv;
  void *return_value_join = 0;

  initBoard();

  //TODO Init own Ship
  pthread_create(&own_ship_tid, NULL, ship, NULL);
  pthread_t enemy;
  pthread_t upgrade;

    //Timer variable
    size_t count = 0;

    do {
        usleep(GAME_DEFAULT_SPEED);
        count++;

        if ((count % 8500) == 0 && curr_enemies > 0)
    {
        //TODO Preparation enemies
        //Attributes or Parameters needed?
        //Look at the code!
        pthread_create(&enemy, NULL, enemies, NULL);
        curr_enemies--;

    }
        if (missed == AMOUNT_ENEMIES) {
        life = 0;
    }

        if ((count % 50000) == 0)
    {
        //TODO Preparation of Upgrades
        //Attributes or Parameters needed?
        //Look at the code!
        pthread_create(&upgrade, NULL, upgradePlacer, NULL);
    }

    printBoard();

  } while (life > 0);

  /* TODO
   *Be sure that all threads are terminated!
   *There is one Thread left, look carefully
   *which one and destroy it*/
     pthread_cancel(own_ship_tid);
    
  

  gameOver();

  /*TODO
   *How to check the status of this particular
   *thread mentioned above?
   *Get the ret value of the terminated Thread!!
   *Use the variable from above!!!*/

  pthread_join(own_ship_tid, &return_value_join);
  
  if (return_value_join == PTHREAD_CANCELED)
    return 0;

  return -1;
}
