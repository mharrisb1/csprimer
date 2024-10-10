#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ALIVE_TOK "██"
#define DEAD_TOK  "░░"

typedef struct {
  int  width;
  int  height;
  int *cells;
} Grid;

Grid *Grid_new(int width, int height) {
  Grid *grid   = malloc(sizeof(Grid));
  grid->width  = width;
  grid->height = height;
  grid->cells  = calloc(width * height, sizeof(int));
  return grid;
}

void Grid_free(Grid *grid) {
  free(grid->cells);
  free(grid);
}

void Grid_swap(Grid **a, Grid **b) {
  Grid *temp = *a;
  *a         = *b;
  *b         = temp;
}

int Grid_get_cell(Grid *grid, int x, int y) {
  return grid->cells[x + grid->width * y];
}

void Grid_set_cell(Grid *grid, int x, int y, int val) {
  grid->cells[x + grid->width * y] = val;
}

void Grid_seed(Grid *grid) {
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      Grid_set_cell(grid, x, y, rand() % 2);
    }
  }
}

int Grid_get_cell_neighbor_count(Grid *grid, int x, int y) {
  int k = 0;
  int i = x + grid->width * y;

  int dy[] = {-1, 0, 1};
  int dx[] = {-1, 0, 1};

  for (int dyi = 0; dyi < 3; dyi++) {
    for (int dxi = 0; dxi < 3; dxi++) {
      if (dy[dyi] == 0 && dx[dxi] == 0) {
        continue; // don't include cell
      }
      if (i < grid->width && dy[dyi] < 0) {
        continue; // can't look up
      }
      if (i % grid->width == 0 && dx[dxi] < 0) {
        continue; // can't look left
      }
      if ((i + 1) % grid->width == 0 && dx[dxi] > 0) {
        continue; // cant' look right
      }
      if (i > ((grid->height - 1) * grid->width) && dy[dyi] > 0) {
        continue; // can't look down
      }
      k += grid->cells[i + (dy[dyi] * grid->width + dx[dxi])];
    }
  }

  return k;
}

Grid *Grid_next(Grid *grid) {
  Grid *grid_next = Grid_new(grid->width, grid->height);
  for (int y = 0; y < grid_next->height; y++) {
    for (int x = 0; x < grid_next->width; x++) {
      int neighbor_k = Grid_get_cell_neighbor_count(grid, x, y);
      if (Grid_get_cell(grid, x, y)) {
        Grid_set_cell(grid_next, x, y, 2 <= neighbor_k && neighbor_k <= 3);
      } else {
        Grid_set_cell(grid_next, x, y, neighbor_k == 3);
      }
    }
  }
  Grid_swap(&grid, &grid_next);
  Grid_free(grid_next);
  return grid;
}

void Grid_display(Grid *grid) {
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      printf("%s", grid->cells[x + grid->width * y] ? ALIVE_TOK : DEAD_TOK);
    }
    printf("\n");
  }
}

int main() {
  int interval_ms = 100;
  int width, height, iterations;
  width = height = iterations = 50;

  Grid *grid = Grid_new(width, height);
  Grid_seed(grid);

  while (iterations) {
    fputs("\033c", stdout);
    Grid_display(grid);
    usleep(interval_ms * 1000);
    grid = Grid_next(grid);
    iterations--;
  }
  Grid_free(grid);
}
