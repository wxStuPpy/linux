#include<func.h>
int main(int argc,char *argv[]){
    int fdw=open(argv[1],O_WRONLY);
    int fdr=open(argv[2],O_RDONLY);
    printf("1 is connetced\n");
}