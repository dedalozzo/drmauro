#ifndef DRMAURO_H
#define DRMAURO_H

#define ROWS 32
#define COLUMNS 16
#define INVALIDE_ROWS 5

#include <stdbool.h>

enum content { EMPTY, VIRUS, PILL };
enum color { RED, YELLOW, BLUE, COLORS_COUNT };
enum command { NONE, RIGHT, LEFT, DOWN, CLOCKWISE_ROTATION, ANTICLOCKWISE_ROTATION };
enum state { RUNNING, VICTORY, DEFEAT };
enum rotation { CLOCKWISE, ANTICLOCKWISE };
enum direction { HORIZONTAL, VERTICAL };


struct halve {
    int row;
    int column;
    enum color color;
};

struct pill {
    enum direction orientation;
    struct halve first_half;
    struct halve second_half;
    bool active;
};

struct cell {
  enum content type;
  enum color color;
  bool to_be_emptied;
};

struct game {
  struct cell grid[ROWS][COLUMNS];
  struct pill pill;
  struct pill tmp_pill;
  enum state status;
  bool refresh_grid;
  int score;
};


void print_grid(struct game *game);
void load_grid(struct game *game, char *path);
void fill_grid(struct game *game, int difficulty);
void refresh_grid(struct game *game);
void execute(struct game *game, enum command command);
enum state victory(struct game *game);

#endif
