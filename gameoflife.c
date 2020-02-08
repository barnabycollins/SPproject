#include<stdio.h>
#include"gol.h"
int main(int argc, char argv[]){
  
  FILE *gridfile = fopen("./glider.txt", "r");
  struct universe v;
  read_in_file(gridfile,&v);


  /* evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  write_out_file(stdout,&v); */
  return 0;
}
