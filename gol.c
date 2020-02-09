#include<stdio.h>
#include<string.h>
#include"gol.h"

void read_in_file(FILE *infile, struct universe *u) {
    unsigned short rowLength = 512;
    char buffer[rowLength];
    char *grid = u->grid;

    // todo check u is big enough for the file
    fgets(buffer, 512, infile);
    buffer[strlen(buffer)-1] = 0;
    strcpy(grid, buffer);
    
    // length of a row
    u->width = rowLength = strlen(buffer);

    while(fgets(buffer, 512, infile)) {
        buffer[rowLength] = 0;
        strcat(grid, buffer);
    }

    u->height = strlen(grid) / u->width;

    int cur = 0;
    printf("\nSuccessfully read in grid:\n");
    while(cur < strlen(grid)) {
        printf("%.*s\n", rowLength, grid + cur);
        cur += rowLength;
    }
    printf("\n");

    fclose(infile);
}

void write_out_file(FILE *outfile, struct universe *u) {
    char *grid = u->grid;
    int cur = 0;

    char output[] = "";

    while (cur < strlen(grid)) {
        fprintf(outfile, "%.*s\n", u->width, grid + cur);
        cur += u->width;
    }

    fclose(outfile);
    printf("Successfully wrote to file!");
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