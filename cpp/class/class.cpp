#include <iostream>
#include <typeinfo>

using std::cout;
using std::endl;

class A
{
public:
    virtual void fun()
    {
        cout << "Afun" << endl;
    }
    void samefun()
    {
        cout << "A1" << endl;
    }
    void samefun(int)
    {
        cout << "A2" << endl;
    }
    // 虚析构函数，确保多态行为
    A(){
        a=1;
    }
    virtual ~A() {} 
    int a;
};

class B : public A
{
public:
    void fun() override
    {
        cout << "Bfun" << endl;
    }
    void samefun()
    {
        cout << "B" << endl;
    }

public:
    using A::samefun;
};
void setA(A&obj){
    obj.a=10;
}
int main(void)
{
    B b;
   // b.samefun(1);
   cout<<b.a<<endl;
   setA(b);
   cout<<b.a<<endl;

    // 基类指针指向派生类对象
    A* basePtr = &b; 

    #if 0
    // 尝试使用 dynamic_cast 进行向下转换
    B* derivedPtr = dynamic_cast<B*>(basePtr);
    if (derivedPtr) {
        cout << "Dynamic cast from A* to B* successful." << endl;
        derivedPtr->fun();
    } else {
        cout << "Dynamic cast from A* to B* failed." << endl;
    }

    // 创建一个基类对象
    A a;
    basePtr = &a;
    // 尝试将指向基类对象的指针转换为派生类指针
    derivedPtr = dynamic_cast<B*>(basePtr);
    if (derivedPtr) {
        cout << "Dynamic cast from A* to B* successful." << endl;
        derivedPtr->fun();
    } else {
        cout << "Dynamic cast from A* to B* failed." << endl;
    }

    try {
        // 基类引用绑定到派生类对象
        A& baseRef = b; 
        // 尝试使用 dynamic_cast 进行向下转换
        B& derivedRef = dynamic_cast<B&>(baseRef);
        cout << "Dynamic cast from A& to B& successful." << endl;
        derivedRef.fun();
    } catch (const std::bad_cast& e) {
        cout << "Dynamic cast from A& to B& failed: " << e.what() << endl;
    }

    try {
        // 基类引用绑定到基类对象
        A& baseRef = a; 
        // 尝试将基类引用转换为派生类引用
        B& derivedRef = dynamic_cast<B&>(baseRef);
        cout << "Dynamic cast from A& to B& successful." << endl;
        derivedRef.fun();
    } catch (const std::bad_cast& e) {
        cout << "Dynamic cast from A& to B& failed: " << e.what() << endl;
    }
    /*基类必须存在虚函数 typeid才有意义*/
    // 使用 typeid 验证类型
    cout << "Type of b: " << typeid(b).name() << endl;
    cout << "Type of *basePtr (pointing to A): " << typeid(*basePtr).name() << endl;
    basePtr = &b;
    cout << "Type of *basePtr (pointing to B): " << typeid(*basePtr).name() << endl;
    #endif
    return 0;
}
    