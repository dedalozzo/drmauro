#ifndef DRMAURO_H
#define DRMAURO_H

#define ROWS 16
#define COLUMNS 8
#define INVALIDE_ROWS 5
#define MIN_ELEMENTS 4

#include <stdbool.h>

enum content { EMPTY, VIRUS, PILL };
enum color { RED, YELLOW, BLUE, BLANK };
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
    int id;
    bool active;
};

struct cell {
  enum content type;
  enum color color;
  int id;
  bool to_be_emptied;
};

struct game {
  struct cell grid[ROWS][COLUMNS];
  struct pill pill;
  struct pill moving_pill;
  int pills_count;
  int virus_count;
  enum state status;
  int score;
  int points_multiplier;
};


void print_grid(struct game *game);
void init_grid(struct game *game);
void load_grid(struct game *game, char *path);
void fill_grid(struct game *game, int difficulty);
void refresh_grid(struct game *game);
void execute(struct game *game, enum command command);
enum state victory(struct game *game);

#endif
