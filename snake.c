#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int displayScore(int score, WINDOW* win) {
  int scoreX = 1;
  int scoreY = 1;
  char scoremsg[10];
  sprintf(scoremsg, "%d", score) ;
  wattron(win, COLOR_PAIR(3));
  mvwaddstr(win, scoreY, scoreX, scoremsg);
  wattroff(win, COLOR_PAIR(3));

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

  int posX = 1;
  int posY = 1;
  int foodX = rand() % 20;
  int foodY = rand() % 20;
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
    posX += dirX;
    posY += dirY;

    wclear(win);
    box(win, 0, 0);

    wattron(win, COLOR_PAIR(1));
    mvwaddstr(win, foodY, foodX, "f");
    wattroff(win, COLOR_PAIR(1));

    wattron(win, COLOR_PAIR(2));
    mvwaddstr(win, posY, posX, "s");
    wattroff(win, COLOR_PAIR(2));

    displayScore(score, win);

    if (posX == foodX && posY == foodY) {
      foodX = rand() % 20;
      foodY = rand() % 20;
      score++;
    }

    if (posX == 0 || posX == COLS - 1 || posY == 0 || posY == LINES - 1) {
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

