#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

void read_in_file(FILE *infile, struct universe *u) {
    unsigned short rowLength = 512;
    char buffer[rowLength];

    fseek(infile, 0L, SEEK_END);
    int fsize = ftell(infile);
    rewind(infile);

    printf("File is %d", fsize);

    char *grid = malloc(fsize * sizeof(char));
    // todo make sure malloc succeeded
    printf("Size %d", sizeof(grid));

    // todo check u is big enough for the file
    fgets(buffer, 512, infile);
    buffer[strlen(buffer)-1] = 0;
    grid = strcpy(grid, buffer);
    
    // length of a row
    u->width = rowLength = strlen(buffer);

    while(fgets(buffer, 512, infile)) {
        buffer[rowLength] = 0;
        grid = strcat(grid, buffer);
    }

    u->grid = grid;
    u->height = strlen(grid) / u->width;

    printf("\nSuccessfully read in grid:\n");

    print_grid(u);
}

void write_out_file(FILE *outfile, struct universe *u) {
    const char *grid = u->grid;
    int cur = 0;

    char output[] = "";

    while (cur < strlen(grid)) {
        fprintf(outfile, "%.*s\n", u->width, grid + cur);
        cur += u->width;
    }

    fclose(outfile);
    printf("\n\nSuccessfully wrote to file!");
}

int is_alive(struct universe *u, int column, int row) {
    return (u->grid[get_index(u, column, row)] == '*') ? 1 : 0;
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
    char *newgrid = malloc(strlen(u->grid));
    
    newgrid = strcpy(newgrid, "");
    
    for (unsigned int i = 0; i < u->height; i++) {
        for (unsigned short j = 0; j < u->width; j++) {
            newgrid = strcat(newgrid, (*rule)(u, j, i) ? "*" : ".");
        }
    }

    u->grid = strcpy(u->grid, "");

    free(u->grid);
    u->grid = newgrid;
}

// SUPPLEMENTARY FUNCTIONS

int get_index(struct universe *u, int column, int row) {
    return row * u->width + column;
}

int sum_surrounding(struct universe *u, int column, int row) {    
    unsigned short total = 0;

    // todo make sure this works for invalid i and j
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

    // todo make sure this works for invalid i and j
    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (i != 0 || j != 0) {
                total += is_alive(u, (column+i) % u->width, (row+j) % u->height);
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

    int cur = 0;
    while(cur < strlen(u->grid)) {
        printf("%.*s\n", u->width, u->grid + cur);
        cur += u->width;
    }

    printf("\n");
}
