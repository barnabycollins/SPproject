#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"gol.h"

// may need changing to int main(int argc, char *argv[]) {
int main(int argc, char argv[]){

  FILE *gridfile = fopen("./glider.txt", "r");
  
  fseek(gridfile, 0L, SEEK_END);
  int fsize = ftell(gridfile);
  rewind(gridfile);

  struct universe *v = malloc(sizeof(*v) + fsize + 1);

  read_in_file(gridfile,v);

  FILE *outfile = fopen("./out.txt", "w");

  write_out_file(outfile,v);

  /* evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  write_out_file(stdout,&v); */
  return 0;
}
