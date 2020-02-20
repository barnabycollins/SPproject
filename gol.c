#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

// private function definitions
int get_index(struct universe *u, int column, int row);
int sum_surrounding(struct universe *u, int column, int row);
int sum_surrounding_torus(struct universe *u, int column, int row);
int check_alive(struct universe *u, int column, int row, unsigned short sur_sum);
void print_grid(struct universe *u);
float get_percent_alive(struct universe *u);
int properMod(int a, int b);
void check_alloc(char *p);


void read_in_file(FILE *infile, struct universe *u) {
    char buffer[513];

    fgets(buffer, 513, infile);
    
    // store length of a row
    u->width = strlen(buffer)-1;

    char *grid = malloc(strlen(buffer));
    check_alloc(grid);

    // do an initial iteration, copying the buffer into grid
    buffer[u->width] = 0;
    strcpy(grid, buffer);

    // iterate over the rest of the lines of the file
    while(fgets(buffer, 513, infile)) {
        grid = realloc(grid, strlen(grid) + strlen(buffer));
        check_alloc(grid);
        
        if(strlen(buffer) != (unsigned) (u->width+1)) {
            fprintf(stderr, "Error: input grid doesn't seem to be rectangular!\n");
            exit(1);
        }

        buffer[u->width] = 0;
        strcat(grid, buffer);
    }

    // write info into the given universe
    u->grid = grid;
    u->height = strlen(grid) / u->width;
    u->avg_alive = get_percent_alive(u);
    u->num_generations = 1;
}

void write_out_file(FILE *outfile, struct universe *u) {
    const char *grid = u->grid;
    unsigned int cur = 0;

    while (cur < strlen(grid)) {
        fprintf(outfile, "%.*s\n", u->width, grid + cur);
        cur += u->width;
    }
}

int is_alive(struct universe *u, int column, int row) {
    // if we're outside the grid, return 0
    // (when this is called in torus mode the column and row will have already been modulo'd)
    if (column < 0 || row < 0 || (unsigned) column >= u->width || (unsigned) row >= u->height) {
        return 0;
    }

    return u->grid[get_index(u, column, row)] == '*';
}

int will_be_alive(struct universe *u, int column, int row) {
    unsigned short sur_sum = sum_surrounding(u, column, row);
    return check_alive(u, column, row, sur_sum);
}

int will_be_alive_torus(struct universe *u, int column, int row) {
    unsigned short sur_sum = sum_surrounding_torus(u, column, row);
    return check_alive(u, column, row, sur_sum);
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)) {
    char *newgrid = malloc(strlen(u->grid)+1);
    check_alloc(newgrid);
    
    newgrid = strcpy(newgrid, "");
    
    for (unsigned int i = 0; i < u->height; i++) {
        for (unsigned short j = 0; j < u->width; j++) {
            newgrid = strcat(newgrid, (*rule)(u, j, i) ? "*" : ".");
        }
    }

    free(u->grid);
    u->grid = newgrid;
    u->avg_alive = (float) (get_percent_alive(u) + u->avg_alive * u->num_generations) / (float) (u->num_generations + 1);
    u->num_generations += 1;
}

void print_statistics(struct universe *u) {
    printf("%.3f%% of cells currently alive\n%.3f%% of cells alive on average", get_percent_alive(u), u->avg_alive);
}

// PRIVATE FUNCTIONS

int get_index(struct universe *u, int column, int row) {
    int index = row * u->width + column;
    return properMod(index, u->width * u->height);
}

int sum_surrounding(struct universe *u, int column, int row) {    
    unsigned short total = 0;

    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (i != 0 || j != 0) {
                total += is_alive(u, column+i, row+j);
            }
        }
    }

    return total;
}

int sum_surrounding_torus(struct universe *u, int column, int row) {

    unsigned short total = 0;

    for (short i=-1; i<=1; i++) {
        for (short j=-1; j<=1; j++) {
            if (!(i == 0 && j == 0)) {
                total += is_alive(u, properMod(column+i, u->width), properMod(row+j, u->height));
            }
        }
    }

    return total;
}

int check_alive(struct universe *u, int column, int row, unsigned short sur_sum) {
    if (2 <= sur_sum && sur_sum <= 3) {
        if (sur_sum == 2 && !is_alive(u, column, row)) {
            return 0;
        }

        return 1;
    }

    return 0;
}

void print_grid(struct universe *u) {
    printf("\n");

    unsigned int cur = 0;
    while(cur < strlen(u->grid)) {
        printf("%.*s\n", u->width, u->grid + cur);
        cur += u->width;
    }

    printf("\n");
}

float get_percent_alive(struct universe *u) {
    unsigned int len = strlen(u->grid);
    unsigned int total = 0;
    for (unsigned int i = 0; i<len; i++) {
        total += (u->grid[i] == '*') ? 1 : 0;
    }

    return 100.0 * ((float) total /(float) len);
}

int properMod(int a, int b) {
    return (a%b+b)%b;
}

void check_alloc(char *p) {
    if (p == NULL) {
        printf(stderr, "Failed to allocate memory to store the universe grid!\n");
        exit(1);
    }
}
