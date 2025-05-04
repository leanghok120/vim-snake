#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int displayScore(int score, WINDOW* win) {
  int scoreX = COLS - 2;
  int scoreY = 1;
  char scoremsg[10];
  sprintf(scoremsg, "%d", score) ;
  wattron(win, COLOR_PAIR(3) | A_BOLD);
  mvwaddstr(win, scoreY, scoreX, scoremsg);
  wattroff(win, COLOR_PAIR(3) | A_BOLD);

  return 0;
}

int displaySnake(int posX, int posY, WINDOW* win) {
  wattron(win, COLOR_PAIR(2));
  mvwaddstr(win, posY, posX, "s");
  wattroff(win, COLOR_PAIR(2));

  return 0;
}

int displayFood(int foodX, int foodY, WINDOW* win) {
  wattron(win, COLOR_PAIR(1));
  mvwaddstr(win, foodY, foodX, "f");
  wattroff(win, COLOR_PAIR(1));

  return 0;
}

int main() {
  WINDOW* win = initscr();

  if (!has_colors()) {
    endwin();
    printf("Your terminal doesn't support colors.\n");
    return 1;
  }

  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  box(win, 0, 0);

  keypad(win, true);
  nodelay(win, true);
  noecho();

  int posX[100] = {1};
  int posY[100] = {1};
  int length = 1;
  srand(time(NULL));
  int foodX = rand() % (COLS - 2) + 1;
  int foodY = rand() % (LINES - 2) + 1;
  int dirX = 1;
  int dirY = 0;
  int score = 0;

  while (true) {
    int key = wgetch(win);
    if (key == 104) {
      dirX = -1;
      dirY = 0;
    }
    if (key == 108) {
      dirX = 1;
      dirY = 0;
    }
    if (key == 107) {
      dirX = 0;
      dirY = -1;
    }
    if (key == 106) {
      dirX = 0;
      dirY = 1;
    }
    int newX = posX[0] + dirX;
    int newY = posY[0] + dirY;

    // shift array
    for (int i = length; i > 0; i--) {
      posX[i] = posX[i - 1];
      posY[i] = posY[i - 1];
    }
    posX[0] = newX;
    posY[0] = newY;

    wclear(win);
    box(win, 0, 0);

    displayScore(score, win);

    for (int i = 0; i < length; i++) {
      displaySnake(posX[i], posY[i], win);
    }

    displayFood(foodX, foodY, win);

    if (posX[0] == foodX && posY[0] == foodY) {
      foodX = rand() % (COLS - 2) + 1;
      foodY = rand() % (LINES - 2) + 1;
      score++;
      length++;
    }

    if (posX[0] == 0 || posX[0] == COLS - 1 || posY[0] == 0 || posY[0] == LINES - 1) {
      wclear(win);
      box(win, 0, 0);

      const char* msg = "GAME OVER - Press any key to exit...";
      int msgX = (COLS - strlen(msg)) / 2;
      int msgY = LINES / 2;

      wattron(win, COLOR_PAIR(3) | A_BOLD);
      mvwaddstr(win, msgY, msgX, msg);
      wattroff(win, COLOR_PAIR(3) | A_BOLD);

      wrefresh(win);
      nodelay(win, false);
      wgetch(win);
      break;
    }

    wrefresh(win);
    usleep(100000);
  }
  
  endwin();

  return 0;
}

