#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

// private function definitions
int get_index(struct universe *u, int column, int row);
int sum_surrounding(struct universe *u, int column, int row);
int sum_surrounding_torus(struct universe *u, int column, int row);
int check_alive(struct universe *u, int column, int row, unsigned short sur_sum);
float get_percent_alive(struct universe *u);
int properMod(int a, int b);
void check_alloc(char *p);
short check_coords(struct universe *u, int column, int row);
void error(char *message);

// todo check over number types

/**
 * Reads the contents of the file at the given file pointer
 * into the structure at the given universe pointer
 */
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
        
        if(strlen(buffer) != (unsigned) (u->width+1)) error("Input grid doesn't seem to be rectangular!");

        buffer[u->width] = 0;
        strcat(grid, buffer);
    }

    // write info into the given universe
    u->grid = grid;
    u->height = strlen(grid) / u->width;
    u->avg_alive = get_percent_alive(u);
    u->num_generations = 1;
}

/**
 * Writes the structure at the given universe pointer
 * into the file at the given file pointer
 */
void write_out_file(FILE *outfile, struct universe *u) {
    unsigned int cur = 0;

    while (cur < strlen(u->grid)) {
        fprintf(outfile, "%.*s\n", u->width, u->grid + cur);
        cur += u->width;
    }
}

/**
 * Checks whether the cell at the given co-ordinates
 * in the given universe is currently alive
 * 
 * Returns 1 if alive and 0 if not
 */
int is_alive(struct universe *u, int column, int row) {
    // if we're outside the grid, return 0
    // (when this is called in torus mode the column and row will have already been modulo'd)
    if (check_coords(u, column, row)) return 0;

    return u->grid[get_index(u, column, row)] == '*';
}

/**
 * Returns whether the cell at the given co-ordinates
 * in the given universe will be alive
 * after a (non-toroidal) evolution
 * 
 * Returns 1 if it will be alive, and 0 if not
 */
int will_be_alive(struct universe *u, int column, int row) {
    if (check_coords(u, column, row)) error("Co-ordinates lie outside the universe grid!");
    
    unsigned short sur_sum = sum_surrounding(u, column, row);
    return check_alive(u, column, row, sur_sum);
}

/**
 * Returns whether the cell at the given co-ordinates
 * in the given universe will be alive
 * after a toroidal evolution
 * 
 * Returns 1 if it will be alive, and 0 if not
 */
int will_be_alive_torus(struct universe *u, int column, int row) {
    if (check_coords(u, column, row)) error("Co-ordinates lie outside the universe grid!");

    unsigned short sur_sum = sum_surrounding_torus(u, column, row);
    return check_alive(u, column, row, sur_sum);
}

/**
 * Evolves the universe at the given pointer using the given rule,
 * writing the evolved universe back into the pointer
 * 
 * Rule should take a universe pointer, a column and row and return
 * 1 if the cell at that location will be alive and
 * 0 if not
 */
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)) {
    char *newgrid = malloc(strlen(u->grid)+1);
    check_alloc(newgrid);
    
    // clear newgrid
    newgrid = strcpy(newgrid, "");
    
    // for each row
    for (unsigned int i = 0; i < u->height; i++) {
        // for each column
        for (unsigned short j = 0; j < u->width; j++) {
            // evolve the current grid cell
            int value = (*rule)(u, j, i);
            if (value != 1 && value != 0) error("Given rule returned invalid value!");
            newgrid = strcat(newgrid, value ? "*" : ".");
        }
    }

    // free up the old grid and point the universe at the new one
    free(u->grid);
    u->grid = newgrid;

    // update average alive information
    u->avg_alive = (float) (get_percent_alive(u) + u->avg_alive * u->num_generations) / (float) (u->num_generations + 1);
    u->num_generations += 1;
}

/**
 * Print statistics about the structure at the given universe pointer
 * 
 * Prints percentage of cells currently alive
 * and percentage of cells alive on average
 */
void print_statistics(struct universe *u) {
    printf("%.3f%% of cells currently alive\n%.3f%% of cells alive on average", get_percent_alive(u), u->avg_alive);
}


// PRIVATE FUNCTIONS

/**
 * Returns the string index at which a certain cell can be found
 * 
 * Gets grid width from the current universe and
 * calculates the location of the cell in the given column and row
 */
int get_index(struct universe *u, int column, int row) {
    return row * u->width + column;
}

/**
 * Finds the number of alive cells in the 8 cells
 * surrounding the cell at the given co-ordinates
 * in the given (non-toroidal) universe
 */
int sum_surrounding(struct universe *u, int column, int row) {    
    unsigned short total = 0;

    for (short i=-1; i<=1; i++) {
        for (short j=-1; j<=1; j++) {
            if (i != 0 || j != 0) {
                total += is_alive(u, column+i, row+j);
            }
        }
    }

    return total;
}

/**
 * Finds the number of alive cells in the 8 cells
 * surrounding the cell at the given co-ordinates
 * in the given toroidal universe
 */
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

/**
 * Returns whether the given cell should be alive
 * in the next generation, given:
 * - cell column and row
 * - number of live cells surrounding it
 */
int check_alive(struct universe *u, int column, int row, unsigned short sur_sum) {
    if (2 <= sur_sum && sur_sum <= 3) {
        if (sur_sum == 2 && !is_alive(u, column, row)) {
            return 0;
        }

        return 1;
    }

    return 0;
}

/**
 * Returns the percentage of cells that are alive
 * in the given universe
 */
float get_percent_alive(struct universe *u) {
    unsigned int len = strlen(u->grid);
    unsigned int total = 0;
    for (unsigned int i = 0; i<len; i++) {
        total += (u->grid[i] == '*') ? 1 : 0;
    }

    return 100.0 * ((float) total /(float) len);
}

/**
 * 'Proper' modulo function
 * 
 * Used in replacement of C's % operator, which finds remainder
 */
int properMod(int a, int b) {
    return (a%b+b)%b;
}

/**
 * Checks that the given string pointer was allocated successfully,
 * and errors if not
 */
void check_alloc(char *p) {
    if (p == NULL) {
        error("Failed to allocate memory to store the universe grid!");
    }
}

/**
 * Returns whether the given co-ordinates exist in the given universe
 * (ie whether they are outside the column and/or row range defined in the universe)
 * 
 * 0 if they do exist
 * 1 if they don't
 */
short check_coords(struct universe *u, int column, int row) {
    return (column < 0 || row < 0 || (unsigned) column >= u->width || (unsigned) row >= u->height);
}

/**
 * Prints the given error to stderr and exits
 */
void error(char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}