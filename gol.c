#include<stdio.h>
#include<string.h>
#include"gol.h"

void read_in_file(FILE *infile, struct universe *u) {
    int rowLength = 512;
    char buffer[rowLength];
    char *grid = u->grid;

    strcpy(grid, "");

    while(fgets(buffer, rowLength, infile)) {
        buffer[strlen(buffer) - 1] = 0;
        strcat(grid, buffer);
    }

    printf("%s", grid);
}