#include <iostream>
#include <string>
using namespace std;

int main() {
    int a = 1;
    int &b = a;
    int &&c = 1;
    const int &d = 1;

    int i = 0;
    int &m = ++i;
    int &&n = i++;

    int ii = 1;
    int &&p = std::move(ii);
    ii = 10;
    cout << "p = " << p << endl;  // 输出10

    string str1 = "abc";
    string str2 = std::move(str1);
    cout << "str1: " << str1 << ", str2: " << str2 << endl; 
    // 可能输出: str1: , str2: abc

    return 0;
}