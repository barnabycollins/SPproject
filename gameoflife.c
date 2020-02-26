#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

/**
 * Prints the given error to stderr and exits
 */
void main_error(char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

/**
 * Called when there's an issue with one of the command line arguments
 */
void arg_error(char *arg) {
    char *msg = malloc(64);
    if (msg != NULL) {
        snprintf(msg, 64, "Unrecognised switch: %s", arg);
        main_error(msg);
    }
    main_error("Unrecognised switch detected!");
}

/**
 * Entrypoint for this program
 */
int main(int argc, char *argv[]) {

    FILE *in = stdin;
    FILE *out = stdout;
    char *inname = "";
    char *outname = "";

    unsigned short print_stats = 0;
    short gens_set = 0;
    unsigned int number_of_generations = 5;
    int (*alive_rule)(struct universe *u, int column, int row) = will_be_alive;

    if (argc > 1) {
        unsigned short i = 1;

        while (i < argc) {
            if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
                switch(argv[i][1]) {
                    case 'i':
                        i++;
                        char *newin = argv[i];
                        if (strcmp(inname, "") != 0 && strcmp(newin, inname) != 0) main_error("Two different inputs specified!");
                        inname = newin;
                        break;
                    
                    case 'o':
                        i++;
                        char *newout = argv[i];
                        if (strcmp(outname, "") != 0 && strcmp(newout, outname) != 0) main_error("Two different outputs specified!");
                        outname = newout;
                        break;
                    
                    case 'g':
                        i++;
                        char *endptr;
                        long int value = strtol(argv[i], &endptr, 10);
                        if (endptr == argv[i]) value = -1;
                        if (value < 0) {
                            main_error("Invalid number of generations!");
                        }
                        unsigned int newgens = (unsigned) value;
                        if (gens_set == 1 && number_of_generations != newgens) main_error("Two different values for number of generations given!");
                        gens_set = 1;
                        number_of_generations = newgens;
                        break;
                    
                    case 's':
                        print_stats = 1;
                        break;
                    
                    case 't':
                        alive_rule = will_be_alive_torus;
                        break;
                    
                    default:
                        arg_error(argv[i]);
                        break;
                }
            }
            else {
                arg_error(argv[i]);
            }
            i++;
        }
    }


    if (strcmp(inname, "") != 0 && !(in = fopen(inname, "r"))) {
        main_error("Given input file doesn't exist!");
    } 

    if (strcmp(outname, "") != 0 && !(out = fopen(outname, "w"))) {
        main_error("Failed to open output file for writing!");
    }

    struct universe v;
    read_in_file(in, &v);
    if (in != stdin) fclose(in);

    for (unsigned int i=0; i<number_of_generations; i++) evolve(&v, alive_rule);

    write_out_file(out,&v);
    if (out != stdout) fclose(out);

    if (print_stats == 1) print_statistics(&v);

    return 0;
}
