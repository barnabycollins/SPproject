#include<stdio.h>
#include<string.h>
#include"gol.h"

void read_in_file(FILE *infile, struct universe *u) {
    unsigned short rowLength = 512;
    char buffer[rowLength];
    char *grid = u->grid;

    fgets(buffer, 512, infile);
    buffer[strlen(buffer)-1] = 0;
    strcpy(grid, buffer);
    
    // length of a row
    u->width = rowLength = strlen(buffer);

    while(fgets(buffer, 512, infile)) {
        buffer[rowLength] = 0;
        strcat(grid, buffer);
    }

    fclose(infile);

    int cur = 0;
    printf("\nSuccessfully read in grid:\n");
    while(cur < strlen(grid)) {
        printf("%.*s\n", rowLength, grid + cur);
        cur += rowLength;
    }
    printf("\n");
}