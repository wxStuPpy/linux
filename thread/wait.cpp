#include<func.h>
int main(int argc,char*argv[]){
    int wstatus;//只能返回0-255
    if(fork()){
        wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("WIFEXITED status = %d\n",WEXITSTATUS(wstatus));
        }
        else if(WIFSIGNALED(wstatus)){
            printf("abnomal quit, status = %d\n",WTERMSIG(wstatus));
        }
    }
    else{
        //while(1);
        //abort();
        return 0; 
    }
}