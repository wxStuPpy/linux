#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,2);
    FILE*ft=fopen(argv[1],"r+");
    ERROR_CHECK(ft,nullptr,"fopen");
}