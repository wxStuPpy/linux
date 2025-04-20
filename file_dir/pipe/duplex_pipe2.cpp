#include<func.h>
int main(int argc,char *argv[]){
    int fdr=open(argv[2],O_RDONLY);
    int fdw=open(argv[1],O_WRONLY);
    printf("2 is connetced\n");
}