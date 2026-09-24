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
    // taking r, c as 0based indexing, so only r is incrementing by 3 row-wise,
    // col 0,1,2 only.
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

bool checkWin(Cell win[3], char player) {
  int lines[8][3][2] = {
      // horizontal line;
      {{0, 0}, {0, 1}, {0, 2}},
      {{1, 0}, {1, 1}, {1, 2}},
      {{2, 0}, {2, 1}, {2, 2}},

      // vertical line;
      {{0, 0}, {1, 0}, {2, 0}},
      {{0, 1}, {1, 1}, {2, 1}},
      {{0, 2}, {1, 2}, {2, 2}},

      // diagonal line;
      {{0, 0}, {1, 1}, {2, 2}},
      {{0, 2}, {1, 1}, {2, 0}},
  };

  for (int i = 0; i < 8; i++) {
    bool lineMatches = true;

    for (int j = 0; j < 3; j++) {
      int r = lines[i][j][0];
      int c = lines[i][j][1];
      if (board[r][c] != player) {
        lineMatches = false;
        break;
      }
    }

    if (lineMatches) {
      for (int j = 0; j < 3; j++) {
        win[j].r = lines[i][j][0];
        win[j].c = lines[i][j][1];
      }
      return true;
    }
  }
  return false;
}

bool isBoardFull(void) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        return false;
      }
    }
  }
  return true;
}

void printWinner(char player) {
  printf(SUCCESS_COLOR " Player " RESET);

  if (player == 'X') {
    printf(X_COLOR "X" RESET);
  } else {
    printf(O_COLOR "O" RESET);
  }

  printf(SUCCESS_COLOR " wins\n\n" RESET);
}

bool askPlayAgain(void) {
  char lines[32];
  printf(SUCCESS_COLOR " Play again? (y/n): " RESET);
  fflush(stdout);

  if (fgets(lines, sizeof(lines), stdin) == NULL) {
    return false;
  }
  return lines[0] == 'y' || lines[0] == 'Y';
}

int main(void) {
  char input[64];
  bool play = true;

  clearScreen();

  printf(TITLE_COLOR "  Press Enter to start..." RESET);
  fflush(stdout);

  if (fgets(input, sizeof(input), stdin) == NULL) {
    return 0;
  }

  while (play) {
    initBoard();

    char player = 'X';
    bool gameOver = false;
    char message[128] = "";
    Cell win[3];

    while (!gameOver) {
      clearScreen();
      printBoard(NULL);

      if (message[0] != '\0') {
        printf(ERROR_COLOR "  %s\n\n" RESET, message);
      }
      printTurn(player);

      printf(PROMPT_COLOR "  >  " RESET);
      fflush(stdout);

      if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
      }

      int a = 0;
      int b = 0;
      int count = sscanf(input, "%d %d", &a, &b);
      int row = -1, col = -1;
      bool valid = false;

      if (count == 1) {
        if (a >= 1 && a <= 9) {
          row = (a - 1) / 3;
          col = (a - 1) % 3;
          valid = true;
        } else {
          snprintf(message, sizeof(message),
                   "Please enter a number from 1 to 9.");
        }
      } else if (count == 2) {
        row = a - 1;
        col = b - 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
          valid = true;
        } else {
          snprintf(message, sizeof(message),
                   "Row and Column must be between 1 and 3.");
        }
      } else {
        snprintf(message, sizeof(message),
                 "Enter one number (1-9), or two numbers like: 2 3.");
      }

      if (!valid) {
        continue;
      }

      if (board[row][col] != ' ') {
        snprintf(message, sizeof(message),
                 "That cell is already taken. Try another one.");
        continue;
      }
      message[0] = '\0';
      board[row][col] = player;

      if (checkWin(win, player)) {
        clearScreen();
        printBoard(win);
        printWinner(player);
        printf("\a");
        gameOver = true;
      } else if (isBoardFull()) {
        clearScreen();
        printBoard(NULL);
        printf(TITLE_COLOR " It's a draw!\n\n" RESET);
        break;
      } else {
        player = (player == 'X') ? 'O' : 'X';
      }
    }

    if (!askPlayAgain()) {
      play = false;
    }

    clearScreen();
  }

  printf(TITLE_COLOR "  Thanks for playing!\n\n" RESET);
  return 0;
}
