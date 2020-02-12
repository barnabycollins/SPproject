#include<stdio.h>
#include<string.h>
#include"gol.h"

// may need changing to int main(int argc, char *argv[]) {
int main(int argc, char argv[]){

	FILE *gridfile = fopen("./glider.txt", "r");

	struct universe v;

	read_in_file(gridfile,&v);

    fclose(gridfile); // todo should this be done in read_in_file()?

	FILE *outfile = fopen("./out.txt", "w");

	write_out_file(outfile,&v);

	printf("\n%d", is_alive(&v, 3, 4));              //1
	printf("\n%d", is_alive(&v, 2, 6));              //1
	printf("\n%d", is_alive(&v, 0, 6));              //0
	printf("\n%d", is_alive(&v, 6, 0));              //0
	printf("\n%d", is_alive(&v, -1, -4));            //0
	printf("\n%d\n", is_alive(&v, 100, 200));        //0

	printf("\n%d\n", sum_surrounding(&v, 3, 5));     //5

	printf("\n%d", will_be_alive(&v, 4, 5));         //1
	printf("\n%d", will_be_alive(&v, 3, 5));         //0
	printf("\n%d", will_be_alive(&v, 3, 7));         //1
	printf("\n%d\n", will_be_alive(&v, 5, 5));       //0

	printf("\n%d\n", will_be_alive_torus(&v, 10, 19)); //1

	evolve(&v,will_be_alive);
	evolve(&v,will_be_alive);
	evolve(&v,will_be_alive);
	evolve(&v,will_be_alive);
	evolve(&v,will_be_alive);

	print_statistics(&v);

	FILE *out2 = fopen("./out2.txt", "w");
	write_out_file(out2,&v);

	return 0;
}
