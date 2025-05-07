#include<iostream>
using namespace std;

class A
{
private:
    
public:
    A(int x,int y,int z=1){
        m_x=x;
        m_y=y;
        m_z=z;
        cout<<"A cons z = "<<m_z<<endl;
    }
    ~A(){

    }
    int m_x;
    int m_y;
    int m_z;
};

class B:public A
{
private:
   
public:
    B(int x=0,int y=1):A(x,y),m_a(x),m_b(y){
        cout<<m_x<<" "<<m_y<<endl;
        cout<<"B cons z = "<<m_z<<endl;
    }
    ~B(){

    }
    int m_b;
    int m_a;
};

int main(){
B b;
}