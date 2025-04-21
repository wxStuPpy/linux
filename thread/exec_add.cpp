#include<func.h>
int main(int argc,char *argv[]){
   ARGC_CHECK(argc,3);
   printf("sum = %d pid = %d\n",atoi(argv[1])+atoi(argv[2]),getpid());
}