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
    int index = row * u->width + column;
    return (u->grid[index] == '*') ? 1 : 0;
}