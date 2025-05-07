#include <iostream>

extern int num;

extern void print();

namespace wd
{
    extern int num;
    extern void print();
}

void test1()
{
    std::cout << num << std::endl;
    print();
}

void test2()
{
    std::cout << wd::num << std::endl;
    wd::print();
}

int main()
{
    test1();
    test2();
    return 0;
}