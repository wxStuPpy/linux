#include<iostream>

int num = 100;

void print(){
    std::cout<<"B"<<std::endl;
}

namespace wd{
    int num=200;
    void print(){
        std::cout<<"wd::B"<<std::endl;
    }
}
