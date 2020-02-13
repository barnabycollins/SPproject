#include<stdio.h>
#include<stdlib.h>
#include"gol.h"

int main(int argc, char *argv[]) {

    FILE *in = stdin;
    FILE *out = stdout;

    unsigned short print_stats = 0;
    unsigned int number_of_generations = 5;
    int (*alive_rule)(struct universe *u, int column, int row) = will_be_alive;

    if (argc > 1) {
        unsigned short i = 1;

        while (i < argc) {
            if (argv[i][0] == '-') {
                switch(argv[i][1]) {
                    case 'i':
                        in = fopen(argv[i+1], "r");
                        i++;
                        break;
                    
                    case 'o':
                        out = fopen(argv[i+1], "w");
                        i++;
                        break;
                    
                    case 'g':
                        number_of_generations = atoi(argv[i+1]);
                        i++;
                        break;
                    
                    case 's':
                        print_stats = 1;
                        break;
                    
                    case 't':
                        alive_rule = will_be_alive_torus;
                        break;
                }
            }
            i++;
        }
    }


    struct universe v;
    read_in_file(in,&v);
    if (in != stdin) fclose(in);

    for (unsigned int i=0; i<number_of_generations; i++) {
        evolve(&v,alive_rule);
    }

    write_out_file(out,&v);
    if (out != stdout) fclose(out);

    if (print_stats == 1) {
        print_statistics(&v);
    }

    return 0;
}
