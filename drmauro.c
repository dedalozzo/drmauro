#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "drmauro.h"


/**
 * @brief Returns a random color between the ones available.
 * @details Such colors are used for virus and pills independently.
 * @return int
 */
int get_random_color() {
  return rand() % BLANK;
}


/**
 * @brief Assigns a new color to the virus at the coordinates (x, y) of the grid.
 * @param game Pointer to the game instance.
 * @param x Position on the x-axis.
 * @param y Position on the y-axis.
 */
void change_virus_color(struct game *game, int x, int y) {
  int current_color = game->grid[x][y].color;
  int new_color;

  do {
    new_color = get_random_color();
  } while (new_color == current_color);

  game->grid[x][y].color = (enum color) new_color;
}


/**
 * @brief Reorganizes the viruses to avoid the presence of two of more consecutive viruses of the same color on the
 * same line. The rule applies to both rows and columns.
 * @param game Pointer to the game instance.
 */
void reorganize_viruses(struct game *game) {
  int x, y;

  int first_row = INVALIDE_ROWS;
  int first_column = 0;

  for (x = first_row; x < ROWS; x++) {
    for (y = first_column; y < COLUMNS; y++) {

      // If the cell is empty, then continue.
      if (game->grid[x][y].type == EMPTY)
        continue;
      else
        game->virus_count++;

      // This is the color of the virus at the coordinates `x`, `y`.
      int color = game->grid[x][y].color;

      // True when there are two consecutive viruses of the same color on the same row or column.
      bool invalid = (game->grid[x][y-1].type == VIRUS && color == game->grid[x][y-1].color &&
                      game->grid[x][y-2].type == VIRUS && color == game->grid[x][y-2].color) ||
                     (game->grid[x-1][y].type == VIRUS && game->grid[x-1][y].color &&
                      game->grid[x-2][y].type == VIRUS && color == game->grid[x - 2][y].color);

      // If the current virus is of the same color of the two previous cells of the same row or column, then change
      // the color of the current cell.
      if (invalid) {
        change_virus_color(game, x, y);
      }
    }
  }
}


/**
 * @brief Returns the correspondent letter to a color.
 * @param color A color of a virus or a pill.
 */
char get_letter_color(enum color color) {
  switch (color) {
    case RED:
      return 'R';
    case YELLOW:
      return 'Y';
    case BLUE:
      return 'B';
    default:
      fprintf(stderr, "Color not supported.\n");
      exit(1);
  }
}


/**
 * @brief Prints the grid.
 * @param game Pointer to the game instance.
 */
void print_grid(struct game *game) {
  // Print the header.
  for (int j = 0; j < COLUMNS; j++)
    printf("=");

  printf("\nGRID\n");

  for (int j = 0; j < COLUMNS; j++)
    printf("=");

  printf("\n");

  // Prints the grid.
  for (int i = 0; i < ROWS; i++) {
    if (i < 10)
      printf("%d  ", i);
    else
      printf("%d ", i);

    // Prints one row of cells.
    for (int j = 0; j < COLUMNS; j++) {
      // If there is a virus in the cell or a pill, then prints the letter correspondent to the virus's color, e.g. `R`.
      // Use uppercase for viruses and lowercase for pills. If the cell is empty, prints `O`.
      switch (game->grid[i][j].type) {
        case VIRUS:
          printf("%c", tolower(get_letter_color(game->grid[i][j].color)));
          break;
        case PILL:
          printf("%c", get_letter_color(game->grid[i][j].color));
          break;
        default:
          printf("#");
          break;
      }
    }

    printf("\n");
  }
}


/**
 * @brief Initially the grid has all empty cells.
 * @param game Pointer to the game instance.
 */
void init_grid(struct game *game) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      game->grid[i][j].id = 0;
      game->grid[i][j].type = EMPTY;
      game->grid[i][j].color = BLANK;
      game->grid[i][j].to_be_emptied = false;
    }
  }
}


/**
  * @brief Loads the grid, reading the layout from a text file, whose name is provided through the command line with
  * the option `-f`.
  * @details The file contains as many rows as the grid. Each row can only have the following characters:\n
  *   - `R` identifies a red virus;\n
  *   - `G` identifies a yellow virus;\n
  *   - `B` identifies a blue virus;\n
  *   - the space identifies an empty cell.\n
  * The function verifies the presence of unwanted characters, in the event that returns an error and halt the program's
  * execution.\n
  * The function acceps the file even in case there are some missing spaces before the line break.\n
  * The schema is then reorganised to prevent that there are two or more consecutive viruses of the same color.
  * It was decided to use this approach, instead of trigger an error for invalid scheme. The specifications,
  * in fact, don't mention any check, assuming the file is always correct, when it might not be.
  * The current implementation provides a better elasticity.
  * @param game Pointer to the game instance.
  * @param path The filepath of the text file.
  */
void load_grid(struct game *game, char *path) {
  FILE *fp = fopen(path, "r");

  // In case the file cannot be opened, displays an error and terminates the exection of the program.
  if (!fp) {
    fprintf(stderr, "Cannot open the file.\n");
    exit(1);
  }

  // Variable used to store the character read.
  int character;

  // X-axis and y-axis coordinates.
  int x = 0, y = 0;

  do {
    character = fgetc(fp);

    switch (character) {
      case 'R':
        game->grid[x][y].type = VIRUS;
        game->grid[x][y].color = RED;
        y++;
        break;
      case 'Y':
        game->grid[x][y].type = VIRUS;
        game->grid[x][y].color = YELLOW;
        y++;
        break;
      case 'B':
        game->grid[x][y].type = VIRUS;
        game->grid[x][y].color = BLUE;
        y++;
        break;
      case ' ':
        y++;
        break;
      case '\n':
        x++;
        y = 0;
        break;
      case EOF:
        break;
      default:
        fprintf(stderr, "The file contains an invalid character.\n");
        exit(1);
    }

  } while (character != EOF);

  fclose(fp);

  reorganize_viruses(game);

  printf("\ninitial grid\n");
  print_grid(game);
}


/**
 * @brief Initializes a vector assigning to each element a value between 0 and 2.
 * @param vector A vector of integers.
 * @param n The vector's dimension.
 */
void generate_viruses(int *vector, int n) {
  for (int i = 0; i < n; i++) {
    // Assigns a number included between 0 and 2 to the cell, who represent the virus color.
    vector[i] = (rand() % 3);
  }
}


/**
 * @brief Shuffle a vector.
 * @param vector A vector of integers.
 * @param n The vector's dimension.
 * @details The function uses the Fisher–Yates shuffle.
 * @see https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
 */
void shuffle_viruses(int *vector, int n) {
  int i, j, tmp;

  for (i = n - 1; i > 0; i--) {
    j = rand() % (i + 1);
    tmp = vector[j];
    vector[j] = vector[i];
    vector[i] = tmp;
  }
}


/**
 * @brief Assign the value `-1` to a number `r` of vector's elements randomly selected.
 * @param vector A vector of integers.
 * @param n Vector's dimension.
 * @param r Number of elements to be removed.
 */
void prune_viruses(int *vector, int n, int r) {
  int k = 0;

  while (k < r) {
    int i = rand() % n;

    if (vector[i] == -1)
      continue;

    vector[i] = -1; // -1 indicates that the virus wasn't removed from the vector.

    k++;
  }
}


/**
 * @brief Assigns the viruses to the grid.
 * @param game Pointer to the game instance.
 * @param vector A vector of integers.
 */
void assign_viruses(struct game *game, const int *vector) {
  int i, j;

  // The first 5 cells of the grid must be empty because they cannot contain viruses.
  for (i = 0; i < INVALIDE_ROWS; i++) {
    for (j = 0; j < COLUMNS; j++)
      game->grid[i][j].type = EMPTY;
  }

  // To the other cells, monsters will be assigned.
  int k = 0;

  for (i = INVALIDE_ROWS; i < ROWS; i++) {
    for (j = 0; j < COLUMNS; j++) {

      if (vector[k] != -1) {
        game->grid[i][j].type = VIRUS;
        game->grid[i][j].color = (enum color) vector[k];
      }
      else {
        game->grid[i][j].type = EMPTY;
      }

      k++;
    }
  }

}


/**
 * @brief Fills the grid with the viruses.
 * @details The viruses are distributed on the grid using the following strategy:\n
 *   - creates a vector with a number of elements equal to the available cells of the matrix, excluding so the first
 *   5 rows of the grid;\n
 *   - initializes the vector assigning to every element a value included between 0 and 2, which represents the virus type;\n
 *   - shuffle the vector using the Fisher-Yates shuffle algorithm;\n
 *   - "removes" from the vector the viruses in surplus choosing randomly to which elements assign the value -1.\n
 *   - verifies there aren't two consecutive viruses on the same line.
 * @param game Pointer to the game instance.
 * @param difficulty Level of difficulty chosen for the game, between 0 and 15.
 * @note The algorithm assumes you cannot have more then two consecutive viruses, of the same type, on the same row or
 * column. The specifications are not very clear on this point, since there is a mention to the word "line". From the
 * images I have inferred the verification is done on both row and column.
 */
void fill_grid(struct game *game, int difficulty) {
  // Verifies that the level of difficulty is between 0 e 15. If not it returns an error.
  assert(difficulty >= 0 && difficulty <= 15);

  // Number of available cells. The first five rows of cells cannot be used.
  const int cell_count = (ROWS * COLUMNS) - (INVALIDE_ROWS * COLUMNS);

  // Number of viruses based on the difficulty and the cells count.
  const int virus_count = cell_count - (4 * (difficulty + 1));

  // Declares a vector with only the cells available on the grid, to be used later to assign the viruses.
  int cells[cell_count];

  generate_viruses(cells, cell_count);
  prune_viruses(cells, cell_count, virus_count);
  shuffle_viruses(cells, cell_count);
  assign_viruses(game, cells);
  reorganize_viruses(game);
}


/**
 * @brief Swap the color of the two halves of a pill.
 * @param pill A pill.
 */
void swap_color(struct pill *pill) {
  enum color tmp = pill->first_half.color;
  pill->first_half.color = pill->second_half.color;
  pill->second_half.color = tmp;
}


/**
 * @brief Marks a group of four or more cells of a line (row or column) having the same color.
 * @details The marked cells can be emptied, all together, in a following step.
 * @param game Pointer to the game instance.
 * @param direction Direction of the line,
 * @param index Index of the line (row o column) to be processed.
 * @param offset Position on the row or column.
 * @param repetitions Number of repetitions.
 */
void mark_cells_for_emptying(struct game *game, enum direction direction, int index, int offset, int repetitions) {

  for (int i = repetitions; i >= 0; i--) {
    switch (direction) {
      case HORIZONTAL:
        game->grid[index][offset - i].to_be_emptied = true;
        break;
      case VERTICAL:
        game->grid[offset - i][index].to_be_emptied = true;
        break;
    }
  }

}


/**
 * @brief Empty the marked cells.
 * @param game Pointer to the game instance.
 * @return bool Returns `true` if any cells have been emptied, `false` otherwise.
 */
bool empty_cells(struct game *game) {
  bool is_changed = false;
  struct cell *cell;
  int virus_killed = 0;


  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      cell = &game->grid[r][c];

      if (cell->to_be_emptied) {

        if (cell->type == VIRUS)
          virus_killed++;

        cell->id = 0;
        cell->type = EMPTY;
        cell->color = BLANK;
        cell->to_be_emptied = false;

        is_changed = true;
      }
    }
  }

  int points = 0;
  for (int i = 1; i <= virus_killed; i++)
    points += (int)(game->points_multiplier * (100 * pow(2, i)));

  if (virus_killed > 0) {
    game->virus_count -= virus_killed;
    game->score += points;
    game->points_multiplier *= 2;
  }

  printf("\nadjacent pills and viruses have been removed from the grid\n");
  print_grid(game);

  return is_changed;
}


/**
 * @brief Process a single line of the grid, (row or column), so that monsters or pills' halves can be eliminated.
 * @param game Pointer to the game instance.
 * @param direction Horizontal for x-axis or vertical for y-axis.
 * @param index Index of the line (row o column) to be processed.
 */
void process_line(struct game *game, enum direction direction, int index) {
  int i = index;

  int limit;
  int repetitions = 0;

  if (direction == HORIZONTAL)
    limit = COLUMNS - 1;
  else
    limit = ROWS - 1;

  for (int j = 0; j < limit; j++) {
    struct cell *next_cell;
    struct cell *current_cell;

    // Determine the current cell and the next one in function of the direction.
    if (direction == HORIZONTAL) {
      current_cell = &game->grid[i][j];
      next_cell = &game->grid[i][j+1];
    }
    else {
      current_cell = &game->grid[j][i];
      next_cell = &game->grid[j+1][i];
    }

    if (current_cell->color == BLANK) {
      repetitions = 0;
      continue;
    }

    if (current_cell->color == next_cell->color) {
      repetitions++;

      if (j+1 == limit && repetitions >= MIN_ELEMENTS-1)
        mark_cells_for_emptying(game, direction, i, limit, repetitions);
    }
    else {
      if (repetitions >= MIN_ELEMENTS-1)
        mark_cells_for_emptying(game, direction, i, j, repetitions);

      repetitions = 0;
    }
  }
}


/**
 * @brief Given the coordinates of a cell, returns the last empty cell row.
 * @param game Pointer to the game instance.
 * @param orientation Horizontal for x-axis or vertical for y-axis.
 * @param row Position on the y-axis.
 * @param column Position on the x-axis.
 * @return int
 */
int get_empty_cell_row_by_column(struct game *game, enum direction orientation, int row, int column) {
  int r = row;
  int c = column;

  while (r < ROWS-1) {

    if (game->grid[r+1][c].type == EMPTY && (orientation == VERTICAL || (orientation == HORIZONTAL && game->grid[r+1][c+1].type == EMPTY)))
      r++;
    else
      break;

  }

  return r;
}


/**
 * @brief Move the pill's halves down to the end of stroke.
 * @param first_half First half of the pill.
 * @param second_half Second half of the pill.
 * @param target_first_half Target cell for the first half of the pill.
 * @param target_second_half Target cell for the second half of the pill.
 */
void move_halves(struct cell *first_half, struct cell *second_half,
                 struct cell *target_first_half, struct cell *target_second_half) {

  target_first_half->id = first_half->id;
  target_first_half->type = first_half->type;
  target_first_half->color = first_half->color;

  first_half->id = 0;
  first_half->type = EMPTY;
  first_half->color = BLANK;

  if (!second_half)
    return;

  target_second_half->id = second_half->id;
  target_second_half->type = second_half->type;
  target_second_half->color = second_half->color;

  second_half->id = 0;
  second_half->type = EMPTY;
  second_half->color = BLANK;
}


/**
 * @brief After the grid has been processed, shakes the grid so the pill's halves can drop till they find a virus, another
 * pill or the bottom of the grid.
 * @param game Pointer to the game instance.
 * @return bool Returns `true` if the grid changed and need to be processed again.
 */
bool shake_grid(struct game *game) {
  bool is_changed = false;

  // Because halves on the row 16 (which is identified as 15, since we start from 0) are already on the bottom of the
  // grid, therefore cannot fall, we start when `i` is equal to 14, so we do not process the last row. We also process
  // the rows from the bottom to the top, so in the inverse order, consider 0,0 is in the top left corner of the grid.
  for (int r = ROWS - 2; r > 0; r--) {
    for (int c = 0; c < COLUMNS; c++) {
      int new_row;

      // We can only drop pill's halves, not the viruses.
      if (game->grid[r][c].type == PILL) {
        // Now we know it's a pill fragment, we need to check if there is any adjacent fragment, which is part of the
        // same pill. To be sure the other half belongs to the same pill, the ID has to match.
        struct cell *first_half = &game->grid[r][c];

        // We have to check if the is there is one part of the pill above, because we start from the bottom.
        if (game->grid[r][c].id == game->grid[r-1][c].id) {
          // If the other half of the pill is above, then the pill can be dropped, without any further check.

          new_row = get_empty_cell_row_by_column(game, VERTICAL, r, c);

          if (new_row != r) {
            struct cell *second_half = &game->grid[r-1][c];
            struct cell *target_first_half = &game->grid[new_row][c];
            struct cell *target_second_half = &game->grid[new_row-1][c];

            move_halves(first_half, second_half, target_first_half, target_second_half);
            is_changed = true;
          }

          continue;
        }

        // In case the other half is not above, can be beside.
        if (c+1 < ROWS && game->grid[r][c].id == game->grid[r][c+1].id) {
          new_row = get_empty_cell_row_by_column(game, HORIZONTAL, r, c);

          if (new_row != r) {
            struct cell *second_half = &game->grid[r][c+1];
            struct cell *target_first_half = &game->grid[new_row][c];
            struct cell *target_second_half = &game->grid[new_row][c+1];

            move_halves(first_half, second_half, target_first_half, target_second_half);
            is_changed = true;

            // We increment `c` so we do not check the next fragment again. This will skip the next column `c+1`, because
            // it has been already processed for the current row `r`.
            //c++;
          }

          // In case the other half is beside, we need to check if cells below the halves are both empty.
          continue;
        }

        // The pill can be dropped because it's constituted of a single fragment.
        new_row = get_empty_cell_row_by_column(game, VERTICAL, r, c);

        if (new_row != r) {
          struct cell *target_first_half = &game->grid[new_row][c];
          move_halves(first_half, NULL, target_first_half, NULL);
          is_changed = true;
        }
      }

    }

  }

  printf("\nthe grid has been shaked\n");
  print_grid(game);

  return is_changed;
}


/**
 * @brief Process the entire grid as consequence of a new command.
 * @param game Pointer to the game instance.
 */
void process_grid(struct game *game) {
  if (game->pill.active)
    return;

  for (int i = ROWS-1; i >= 0; i--)
    process_line(game, HORIZONTAL, i);

  for (int i = 0; i < COLUMNS; i++)
    process_line(game, VERTICAL, i);

  // If the pill didn't kill any viruses, or it didn't clear other pills, then the function return, because there is no
  // need for shaking and processing.
  if (!empty_cells(game))
    return;

  // Call itself recursively if there was any change in the grid.
  if (shake_grid(game)) {
    process_grid(game);
  }

  // After we have shaken the grid, even multiple times, the multiplier has to be reinstated to the initial value.
  game->points_multiplier = 1;

  if (game->virus_count == 0) {
    game->status = VICTORY;
    return;
  }
}


/**
 * @brief Removes the current pill from the grid.
 * @param game Pointer to the game instance.
 */
void remove_active_pill_from_grid(struct game *game) {
  struct pill *p = &game->pill;

  if (!p->active)
    return;

  game->grid[p->first_half.row][p->first_half.column].id = 0;
  game->grid[p->second_half.row][p->second_half.column].id = 0;
  game->grid[p->first_half.row][p->first_half.column].type = EMPTY;
  game->grid[p->second_half.row][p->second_half.column].type = EMPTY;
  game->grid[p->first_half.row][p->first_half.column].color = BLANK;
  game->grid[p->second_half.row][p->second_half.column].color = BLANK;
}


/**
 * @brief Restores the current pill on the grid.
 * @param game Pointer to the game instance.
 */
void restore_active_pill_to_grid(struct game *game) {
  struct pill *p = &game->pill;

  if (!p->active)
    return;

  game->grid[p->first_half.row][p->first_half.column].id = p->id;
  game->grid[p->second_half.row][p->second_half.column].id = p->id;
  game->grid[p->first_half.row][p->first_half.column].type = PILL;
  game->grid[p->second_half.row][p->second_half.column].type = PILL;
  game->grid[p->first_half.row][p->first_half.column].color = p->first_half.color;
  game->grid[p->second_half.row][p->second_half.column].color = p->second_half.color;
}


/**
 * @brief Refreshes the grid because the pill moved.
 * @param game Pointer to the game instance.
 */
void refresh_grid(struct game *game) {
  struct pill *pill = &game->pill;
  struct pill *moving_pill = &game->moving_pill;

  if (!pill->active )
    return;

  // New coordinates of the pill's halves.
  int r1 = moving_pill->first_half.row;
  int c1 = moving_pill->first_half.column;
  int r2 = moving_pill->second_half.row;
  int c2 = moving_pill->second_half.column;

  // Removes the pill from the grid, to restore it after in a different position.
  remove_active_pill_from_grid(game);

  // If the pill is outside the grid perimeter, then the command is invalid, therefore the pill is restored on the grid
  // and the control returns to the caller function.
  if (r1 > ROWS - 1 || c1 > COLUMNS - 1 || r2 > ROWS - 1 || c2 > COLUMNS - 1 || c1 < 0 || c2 < 0) {
    restore_active_pill_to_grid(game);
    return;
  }

  // The command is invalid even if only one half of the pill occupies a cell that is not empty.
  // (r1, c1) e (r2, c2) are the coordinates of the cells that the pills should occupy.
  // The check `r2 >= 0` is there to avoid that the program terminates, in which case the pill is vertical oriented and
  // exceeds the grid. `r2`, in fact, could be equal to `-1`, which is outside the grid perimeter.
  if ((game->grid[r1][c1].type != EMPTY) || (r2 >= 0 && game->grid[r2][c2].type != EMPTY)) {

    // In such a case a further check has to be done to be sure the pill is exactly in the middle of the first row. If
    // so, in virtue of the fact the cells are already taken, the game is over, therefore the state of the game doesn't
    // change.
    if (r1 == 0 && c1 == (COLUMNS / 2) - 1)
      game->status = DEFEAT;

    restore_active_pill_to_grid(game);
    return;
  }

  // REPOSITION THE PILL

  // Inserts the first half of the pill on the grid.
  game->grid[r1][c1].id = moving_pill->id;
  game->grid[r1][c1].type = PILL;
  game->grid[r1][c1].color = moving_pill->first_half.color;

  // Proceeds with the second half if inside the grid.
  game->grid[r2][c2].id = moving_pill->id;
  game->grid[r2][c2].type = PILL;
  game->grid[r2][c2].color = moving_pill->second_half.color;

  // A pill gets deactivated when reaches the end of stroke, therefore in the following cases:
  //   - it's at the bottom, namely the last row of the grid;
  //   - below the first half there is not an empty cell, but a virus or a pill;
  //   - when the pill is horizontal and below the second half there is not an empty cell.
  if (r1 == ROWS - 1 ||
      (game->grid[r1+1][c1].type != EMPTY) ||
      (moving_pill->orientation == HORIZONTAL && game->grid[r2 + 1][c2].type != EMPTY)) {
    moving_pill->active = false;
  }

  // Assigns to the active pill, the copy.
  memcpy(pill, moving_pill, sizeof(struct pill));

  game->status = RUNNING;

  process_grid(game);
}


/**
 * @brief Rotates the pill clockwise or anti-clockwise.
 * @details The direction occurs maintaining the x-axis constant, when possible.
 * @param game Pointer to the game instance.
 * @param direction The direction of rotation. A pill can rotate clockwise or anti-clockwise.
 */
void rotate_pill(struct game *game, enum rotation direction) {
  if (!game->pill.active)
    return;

  // Assigns the pill to a temporary variable.
  struct pill temp = game->pill;

  switch (game->pill.orientation) {
    case HORIZONTAL:
      // The first half of the pill remains meanwhile the second half is moved in the row above the first half.
      temp.second_half.row--;
      temp.second_half.column--;

      if (direction == CLOCKWISE)
        swap_color(&temp);

      temp.orientation = VERTICAL;

      break;
    case VERTICAL:
      temp.second_half.row++;
      temp.second_half.column++;

      // If the second half of the pill ends on an occupied cell, then shift to the left the entire pill.
      if (temp.second_half.column == COLUMNS || game->grid[temp.second_half.row][temp.second_half.column].type != EMPTY) {
        temp.first_half.column--;
        temp.second_half.column--;
      }

      if (direction == ANTICLOCKWISE)
        swap_color(&temp);

      temp.orientation = HORIZONTAL;

      break;
  }

  game->moving_pill = temp;
}


/**
 * @brief Moves the pill.
 * @details The player can move the pill to left of the right, or make it fall towards the bottom over another pill, a
 * virus or the bottom of the grid.
 * @param game Pointer to the game instance.
 * @param direction Direction of the pill.
 */
void move_pill(struct game *game, enum command direction) {
  if (!game->pill.active)
    return;

  // Use a temporary variable to store the pill.
  struct pill temp = game->pill;

  switch (direction) {

    case RIGHT:
      temp.first_half.column++;
      temp.second_half.column++;
      break;

    case LEFT:
      temp.first_half.column--;
      temp.second_half.column--;
      break;

    case DOWN: {
      int i = temp.first_half.row + 1;

      while (i < ROWS) {
        // If there is no place for the pill then it stops.
        if (game->grid[i][temp.first_half.column].type != EMPTY || game->grid[i][temp.second_half.column].type != EMPTY)
          break;

        i++;
      }

      // Move the pill down.
      temp.first_half.row = i - 1;

      if (temp.orientation == HORIZONTAL)
        temp.second_half.row = i - 1;
      else
        temp.second_half.row = i - 2;
    }
      break;

    case NONE:
      temp.first_half.row++;
      temp.second_half.row++;
      break;

    default:
      return;
  }

  game->moving_pill = temp;
}


/**
 * @brief Creates a new pill.
 * @details The orientation of a new pill is always horizontal.
 * @param game Pointer to the game instance.
 */
void create_pill(struct game *game) {
  game->pill.orientation = HORIZONTAL;

  // The x-axis is `-1` because the pill is positioned in the first valid raw of the grid.
  game->pill.first_half.row = -1;
  game->pill.second_half.row = -1;

  // The pill is positioned at the middle of the grid y-axis.
  game->pill.first_half.column = (COLUMNS / 2) - 1;
  game->pill.second_half.column = game->pill.first_half.column + 1;

  // A random color is assigned to the pill.
  game->pill.first_half.color = (enum color) get_random_color();
  game->pill.second_half.color = (enum color) get_random_color();

  // DEBUG ONLY
  //game->pill.first_half.color = (enum color) RED;
  //game->pill.second_half.color = (enum color) RED;

  game->pills_count++;

  // Every pill must have an identifier. It is used to identify two halves of the same pill in the grid.
  game->pill.id = game->pills_count;

  game->pill.active = true;

  // When we create a new pill, we also move it at the center top of the grid, therefore the moving pill coincides with
  // the pill.
  game->moving_pill = game->pill;
}


/**
 * @brief Executes a command.
 * @details A player can move the pill to the left or right, rotate it clockwise or anti-clockwise, drop it towards the
 * bottom of the grid. In case there is no input, the program will drop the pill on one position at the time. If there
 * is not an active pill, one will be created and positioned in the middle of the first valid row.
 * @param game Pointer to the game instance.
 * @param command Command given by the player.
 */
void execute(struct game *game, enum command command) {

  switch (command) {
    case RIGHT:
      move_pill(game, RIGHT);
      break;

    case LEFT:
      move_pill(game, LEFT);
      break;

    case DOWN:
      move_pill(game, DOWN);
      break;

    case CLOCKWISE_ROTATION:
      rotate_pill(game, CLOCKWISE);
      break;

    case ANTICLOCKWISE_ROTATION:
      rotate_pill(game, ANTICLOCKWISE);
      break;

    case NONE:
      if (!game->pill.active)
        create_pill(game);
      move_pill(game, NONE);
      break;

    default:
      break;
  }

  refresh_grid(game);
}


/**
 * @brief Returns the game state.
 * @return state
 * @param game Pointer to the game instance.
 */
enum state victory(struct game *game) {
  return game->status;
}
