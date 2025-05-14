#include <iostream>
#include <map>

template <class T, int size>
class A
{
private:
public:
    using A_type=T;
    A();
    ~A();
    A &operator=(const A &);
    int fun(A &);
    A_type& typeFun(const A&);
};

template <class T, int size>
A<T, size>::~A()
{
}

template <class T, int size>
int A<T, size>::fun(A &)
{
    return 0;
}

template <class T, int size>
A<T, size>::A()
{
}

/*返回A的实例化也必须加上模版参数*/
template <class T, int size>
A<T, size> &A<T, size>::operator=(const A &other)
{
    return *this;
}

template <class T, int size>
typename A<T,size>::A_type& typeFun(const A<T,size>&){
    
}


template<class TT>
using mymap=std::map<int,TT>;

int main()
{
    A<int, 3> a;
    A<int, 3> b;
    a = b;
    mymap<double>hashMap;
    return 0;
}