#include<iostream>
using namespace std;
int add(int a,int b){
    return a+b;
}

/*静态库 g++ -c add.c -o add.o 即使删除静态库 也不会影响已经使用库的代码
ar crsv libadd.a add.o
sudo cp libadd.a /usr/lib
g++ main.o -o main -ladd*/

/*动态库 g++ -c add.c -o add.o -fpic 程序启动时 动态库必须存在
g++ -shared add.o -o libadd.so
sudo cp libadd.so /usr/lib
g++ main.o -o main -ladd*/

/*产品更新->升级动态库*/
//ln -s libadd.so.0.0 libadd.so