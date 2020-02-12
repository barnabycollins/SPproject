struct universe {
    unsigned short width;
    unsigned int height;
    char *grid;
    float avg_alive;
    unsigned int num_generations;
};

/*Do not modify the next seven lines*/
void read_in_file(FILE *infile, struct universe *u);
void write_out_file(FILE *outfile, struct universe *u);
int is_alive(struct universe *u, int column, int row);
int will_be_alive(struct universe *u, int column, int row);
int will_be_alive_torus(struct universe *u,  int column, int row);
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row));
void print_statistics(struct universe *u);
/*You can modify after this line again*/

int get_index(struct universe *u, int column, int row);
int sum_surrounding(struct universe *u, int column, int row);
int sum_surrounding_torus(struct universe *u, int column, int row);
int check_alive(struct universe *u, int column, int row, unsigned short sur_sum);
void print_grid(struct universe *u);
float get_percent_alive(struct universe *u);