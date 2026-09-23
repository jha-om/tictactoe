// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
#include <stdbool.h>
#include <stdio.h>
// \033 is escape sequence
// [0m => to reset the pre-existing formats of terminal if any
// [1m => to bold the text
// [2m => to dim the text
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"

#define TITLE_COLOR "\033[1;38;5;220m"
#define BOARD_COLOR "\033[38;5;99m"
#define X_COLOR "\033[1;38;5;203m"
#define O_COLOR "\033[1;38;5;51m"
#define INFO_COLOR "\033[38;5;252m"
#define PROMPT_COLOR "\033[1;38;5;214m"
#define ERROR_COLOR "\033[1;38;5;196m"
#define SUCCESS_COLOR "\033[1;38;5;46m"
#define WIN_HIGHLIGHT "\033[1;30;42m"

typedef struct {
  int r;
  int c;
} Cell;

char board[3][3];

void initBoard(void) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
}

void clearScreen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

void printHeader(void) {
  printf("\n");
  printf(TITLE_COLOR " ===========================================\n" RESET);

  printf(TITLE_COLOR "          " X_COLOR "T I C  T A C  T O E" TITLE_COLOR
                     "          \n" RESET);
  printf(TITLE_COLOR " ===========================================\n\n" RESET);
}

bool isWinningCell(Cell win[3], int r, int c) {
  if (win == NULL) {
    return false;
  }
  for (int i = 0; i < 3; i++) {
    if (win[i].r == r && win[i].c == c) {
      return true;
    }
  }
  return false;
}

void printCell(Cell win[3], int r, int c) {
  char ch = board[r][c];
  if (isWinningCell(win, r, c)) {
    printf(WIN_HIGHLIGHT " %c " RESET, ch);
    return;
  }

  if (ch == 'X') {
    printf(X_COLOR " X " RESET);
  } else if (ch == 'O') {
    printf(O_COLOR " O " RESET);
  } else {
    // taking r, c as 0based indexing, so only r is incrementing by 3 row-wise, col 0,1,2 only.
    printf(DIM " %d " RESET, r * 3 + c + 1);
  }
}

void printBoard(Cell win[3]) {
  printHeader();
  printf(BOARD_COLOR "      1   2   3\n" RESET);
  printf(BOARD_COLOR "    ┌───┬───┬───┐\n" RESET);

  for (int i = 0; i < 3; i++) {
    printf(BOARD_COLOR "  %d │" RESET, i + 1);
    for (int j = 0; j < 3; j++) {
      // internal highlighting (X,O, NUMBER);
      printCell(win, i, j);

      if (j < 2) {
        printf(BOARD_COLOR "|" RESET);
      }
    }

    printf(BOARD_COLOR "|\n" RESET);

    if (i < 2) {
      printf(BOARD_COLOR "    ├───┼───┼───┤\n" RESET);
    }
  }
  printf(BOARD_COLOR "    └───┴───┴───┘\n\n" RESET);
}

void printTurn(char player) {
  printf(INFO_COLOR " Current turn: " RESET);

  if (player == 'X') {
    printf(X_COLOR "X" RESET);
  } else {
    printf(O_COLOR "O" RESET);
  }
  printf("\n");
  printf(INFO_COLOR "  Enter " BOLD "1-9" RESET INFO_COLOR " or " BOLD
                    "row col" RESET INFO_COLOR " like " BOLD
                    "2 3" RESET INFO_COLOR "\n\n" RESET);
}

int main(void) {
  initBoard();
  printBoard(NULL);
  printTurn('X');
  return 0;
}
