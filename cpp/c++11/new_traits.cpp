#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<initializer_list>
#include<memory>//智能指针头文件
#include<functional>

using namespace std;

class A {
public:
	int a;
	const static int b = 10;
	string c;
	static int AA() { return 1; }
	A() { a = 10; c = "a"; }
};

//1.原始字面量
void test01() {
	string str1 = R"a(\hello\world\teach)a";//a仅仅起修饰的作用 两边修饰的内容必须一样
	string str2 = R"(/head
wadawd
\2ewea\da
\ttt)";
	cout << str1 << endl;
	cout << str2 << endl;
}

//2.constexpt(可以提高程序运行效率 可以直接将表达式定义为常量 在编译阶段就可以得到最终表达式类型)(类似宏定义)
void test02() {
	constexpr int a = 10;
	//a = 20; error a是常量
}
//修饰函数 必须写在函数返回值前面 且必须返回常量
//constexpr void fun1(); error void没有返回值
constexpr int fun02_1() {
	// int a = 10;error 必须返回常量
	constexpr int a = 10;
	return a;
}
//在函数的声明和定义必须都要在使用函数之前 在这个函数体中不能出现非常量内容(using return static_assert等除外)
constexpr int fun02_2() {
	using type = int;
	constexpr type a = 10;
	constexpr type b = 10;
	return a + b;
	/*for (int i = 0; i < 10; ++i)
		cout << a;*/ //error 不允许出现非常量内容
}

//构造函数必须使用初始式
class test02_class {
	constexpr test02_class() :a(100) {};//必须使用初始式 同时必须空实现
	int a;
};

//3.auto decltype
//当变量不是指针或者引用类型时 自带的const和volatile关键字会被过滤掉
void test03() {
	auto x = 3.14;
	//auto n;error 必须赋初始值才能推导出来
	int temp = 1;
	auto* a = &temp;//auto=int
	auto b = &temp;//auto=int*
	auto& c = temp;//auto=int

	const auto d = temp;
	auto e = d;//由于e不是指针或者引用 不会保留d的const auto=int;
	auto& f = d;//引用 保留const
	auto* p = &d;//指针 保留const
}
//auto的限制
//不能推导形参 fun(auto a)
//不能用于类的非静态成员的初始化
class test03_class {
	//auto a1; error
	//static auto a2 = 10; 类的非常量成员不能在类内初始化
	const static auto a3 = 0;//ok
};
//不能定义数组
void test03_1() {
	int arr[2] = { 1,2 };
	auto p = arr;//ok
	//auto pp[2] = { 1,2 };error
}
//无法推导出模版类型
template<class T>
struct test03_struct {
	test03_struct() {};
};
void test03_2() {
	test03_struct<double>t;
	//test03_struct<auto>t1 = t; error
}
//推荐使用auto ①迭代器②泛型编程
class test03_T1 {
public:
	const static int a = 0;
	static int t3_T() {
		return 1;
	}
};
class test03_T2 {
public:
	static char t3_T() {
		return 'a';
	}
};

template<class T>
void test03_3() {
	auto it = T::t3_T();
	cout << it << endl;
}

//4.decltype

//int _4fun1() { return 0; }
//int& _4fun2() { int a = 0; return a; }//返回了临时变量的引用 要修改 此处只作为示例
//int&& _4fun3() { return 0; }//返回了临时变量的引用 要修改 此处只作为示例
//const int _4fun4() { return 3; }
//const int& _4fun5() { int a = 0; return a; }//返回了临时变量的引用 要修改 此处只作为示例
//const int&& _4fun6() { return 0; }//返回了临时变量的引用 要修改 此处只作为示例
//const A _4fun7() { return A(); }
//void test04() {
//	//与括号内的变量或表达式的类型保持一致
//	//int a = 5;
//	//decltype(a)b = 10;//int
//	//decltype(a * 3.14)c = 23.4;//double
//	//cout << sizeof(b) << " " << sizeof(c);
//
//	int x = 1;
//	const int& y = x;
//	decltype(y)z = x;//const int&
//	decltype(test03_T1::a)j = 10;//const int
//
//	//如果括号内为右值 忽略const
//	int nn = 0;
//	decltype(_4fun1())a = 0;//int
//	decltype(_4fun2())b = nn;//int&
//	decltype(_4fun3())c = 0;//int&&
//	decltype(_4fun4())d = 0;//int 返回值如果为右值(具体的数 字符串常量等) 忽略返回值的const
//	decltype(j + 10)k = 0;//int j+10为右值 忽略const
//	decltype(_4fun5())e = nn;//const int&
//	decltype(_4fun6())f = 0;//const int&&
//	decltype(_4fun7())g = A();//const A
//
//	//如果表达式是一个左值或者被()包裹 那么推导出的类型时对应类型的引用
//	//如果括号内为左值(可以进行取地址操作的值) 不能忽略const 加了小括号要判断是否有const关键字
//	const A obj;
//	decltype(obj.a)h = 0;//int
//	decltype((obj.a))i = a;//const int& 小括号代表引用 且会保留const
//	int m = 0, n = 0;
//	decltype(m + n)l = 0;//int
//	decltype(n = n + m)o = m;//int &
//	decltype(n++)p = m;//int
//	decltype(++n)q = m;//int &
//}
//decltype的应用
template<class T>
class Container {
public:

	void __4printSingle(T& m) {
		for (it = m.begin(); it != m.end(); ++it)
			cout << (*it) << " ";
		cout << endl;
	}
	void __4printDouble(T& m) {
		for (const auto& element : m) {
			cout << element.first << " : " << element.second << " ";
		}
		cout << endl;
	}
private:
	decltype(T().begin())it;//匿名对象的迭代器类型就是it的类型
	// auto static it = T().begin();//error 类内直接赋值 同时auto不能初始化it
};

//5.函数返回值类型后置
template<class T1, class T2>
//T add(T1 a, T2 b) {
//	return a + b;
//}
auto add(T1 a, T2 b) -> decltype(a + b) {
	return 2 * a + b;
}
void test05() {
	int a = 5;
	double b = 1.2;
	//auto ret=add<decltype(a + b), int, double>(a, b);  decltype(a + b)不太合理因为我们已经知道了最终返回a+b;
	auto ret = add<int, double>(a, b);
	auto ret1 = add(a, b);//可以不需要int 与 double
	cout << "ret==" << ret << "ret1==" << ret1;
}

//6.模版的右尖括号优化与默认模版参数
template<class T1 = int, class T2 = long >
void test06_1(T1 t1 = 'a', T2 t2 = 'b') {
	//根据传入的a自动推导出T的类型==a的类型
	//指定类型>推导类型>默认类型
	cout << t1 << " " << t2 << endl;
}
void test06() {
	test06_1<int, int>('a', 'b');//int int
	test06_1<int>('a', 'b');//int char
	test06_1();//int long
}

//7.using typedef的作用
//using调用父类隐藏的函数
class Base {
public:
	void func(int)const {
		cout << "Base::func()" << endl;
	}
};
class Derived : public Base {
public:
	using Base::func; // 将父类的 func 函数引入到子类的作用域
	void func(double)const {
		cout << "Derived::func()" << endl;
	}
	void callBaseFunc() {
		func(1.0); // 调用 Derived::func()
		Base::func(1); // 直接调用父类的 func 函数
		func(1); // 调用 Base::func()，因为使用了 using 将其引入到了 Derived 的作用域 则存在对象形参为int类型的父类函数
	}
};
//给已有的类型起别名
using _uint1 = unsigned int;
typedef unsigned int _unit2;

//定义函数指针
double mytest07(const int& a, const int& b) {
	return a + b;
}
typedef double (*func1)(const int&, const int&);
using func2 = double(*)(const int&, const int&);

//给模版定义别名 typedef比较麻烦 using可读性强
template<class T>
struct MyMap {
	typedef map<int, T>mapType;
};

template<class T>
using _MyMap = map<int, T>;

void test07() {
	/*{
		func1 f1 = mytest07;
		int a = 0;
		cout << f1(3, a) << endl;
		func2 f2 = mytest07;
		cout << f2(2, a) << endl;
		cout << "解引用方式调用" << endl;
		cout << (*f1)(3, a) << endl;
		cout << (*f2)(2, a) << endl;
	}*/
	{
		MyMap<string>::mapType m1;
		pair<int, string>p1(1, "a");
		m1.insert(p1);
		MyMap<double>::mapType m2;
		m2.insert(make_pair(1, 2.0));
		Container<MyMap<string>::mapType>c;
		c.__4printDouble(m1);
		_MyMap<char>m3;
		m3.insert(make_pair(1, 'c'));
		Container<_MyMap<char>>c1;
		c1.__4printDouble(m3);
	}
}

//8.委托构造函数  用构造函数给其他重载的构造函数初始化
//class _8 {
//	int _a;
//	int _b;
//	int _c;
//	_8(){}
//	//_8(int a){ _a(a); } error 这样赋值会使得_a像一个函数名而产生歧义
//	_8(int a){ _a=a; }
//	//_8(int a,int b):_8(a), _b(b) {}error 委托构造函数不能具有其他成员初始化表达式
//	_8(int a, int b) :_8(a) { _b = b; }
//	_8(int a, int b, int c) :_8(a, b) { _c = c; }
//};

//9.继承构造函数
class Base9 {
public:
	Base9(int x) : value(x) {
		cout << "Base 构造函数被调用,value = " << value << endl;
	}
	Base9(double x) : value(static_cast<int>(x)) {
		cout << "Base 构造函数被调用,value = " << value << endl;
	}
	int value;
};
class Derived9 : public Base9 {
public:
	using Base9::Base9;  // 继承 Base 类的构造函数
	void printValue() {
		cout << "Derived 中的 value = " << value << endl;
	}
};
void test09() {
	Derived9 d1(5);  // 调用继承自 Base 的 Base(int) 构造函数
	d1.printValue();
	Derived9 d2(3.14);  // 调用继承自 Base 的 Base(double) 构造函数
	d2.printValue();
}

//10.initializer_list模版类
void printList(initializer_list<int> lst) {
	for (const auto& elem : lst) {
		cout << elem << " ";
	}
	cout << endl;
}
class MyClass10 {
private:
	initializer_list<int> data;
public:
	// 构造函数接受 initializer_list 作为参数
	MyClass10(initializer_list<int> lst) : data(lst) {
		cout << "MyClass 构造函数被调用" << endl;
	}
	// 打印存储在 initializer_list 中的元素
	void printData() const {
		for (const auto& elem : data) {
			cout << elem << " ";
		}
		cout << endl;
	}
};
void test10() {
	// 使用 initializer_list 作为函数参数
	printList({ 1, 2, 3, 4, 5 });//利用大括号来初始化
	// 使用 initializer_list 作为类的构造函数参数
	MyClass10 obj({ 10, 20, 30 });
	obj.printData();
	// 直接初始化 initializer_list
	initializer_list<int> myList = { 100, 200, 300 };
	for (const auto& elem : myList) {
		cout << elem << " ";
	}
	cout << endl;
}

//11.基于范围的for循环
void rangeBasedForExamples() {
	// 示例 1: 遍历数组
	cout << "遍历数组: ";
	int arr[] = { 1, 2, 3, 4, 5 };
	for (const auto& element : arr) {
		cout << element << " ";
	}
	cout << endl;

	// 示例 2: 遍历 vector
	cout << "遍历 vector: ";
	vector<int> vec = { 10, 20, 30, 40, 50 };
	for (const auto& element : vec) {
		cout << element << " ";
	}
	cout << endl;

	// 示例 3: 遍历 string
	cout << "遍历 string: ";
	string str = "Hello, World!";
	for (const char& ch : str) {
		cout << ch;
	}
	cout << endl;

	// 示例 4: 遍历 map
	cout << "遍历 map: ";
	map<int, string> myMap = { {1, "one"}, {2, "two"}, {3, "three"} };
	for (const auto& [key, value] : myMap) {
		cout << key << " : " << value << " ";
	}
	cout << endl;

	// 示例 5: 修改容器元素（使用非 const 引用）
	cout << "修改 vector 元素: ";
	vector<int> modifiableVec = { 1, 2, 3, 4, 5 };
	for (auto& element : modifiableVec) {
		element *= 2;
	}
	for (const auto& element : modifiableVec) {
		cout << element << " ";
	}
	cout << endl;

	// 示例 6: 遍历 set 容器（只读）
	cout << "遍历 set 容器: ";
	set<int> mySet = { 5, 4, 3, 2, 1 };
	for (auto& element : mySet) {
		cout << element << " ";
		//element++; error set容器内容默认只能读
	}
	cout << endl;
}
//调用次数问题
vector<int>v11 = { 1,2,3,4,5 };
int count_ = 0;
vector<int>& fun11() {
	count_++;
	return v11;
}
void test11() {
	//rangeBasedForExamples();
	//fun11()只会被展开一次 后续不需要
	for (const auto& element : fun11())
	{
		//如果对元素进行操作 则会导致边界出现异常 一次的展开就无法正确遍历
	}
	//count_==1;
	//每次都要对比v11.end()
	for (auto it = v11.begin(); it != v11.end(); ++it) {
	}
}

//12.可调用对象
// 一、函数指针 int (*pfun)()=&fun;
// 二、仿函数（函数对象）
class Functor {
public:
	void operator()(int x) const {
		cout << "Functor called with value: " << x << endl;
	}
};
//三、可转换为函数指针的类(静态函数)
//四、 类函数指针或类的成员指针
class MyClass12 {
public:
	using funcptr = void(*)(int, string);
	void hello(int x, string s) {
		cout << "Function-like object called with value: " << x << " " << s << endl;
	}
	static void world(int x, string s) {
		cout << "Function-like object called with value: " << x << " " << s << endl;
	}
	operator funcptr() const {
		return world;
	}
	void operator()(string b) {
		cout << " " << b << endl;
	}
	int id = 0;
};
void test12() {
	//类的函数指针
	using fptr = void(MyClass12::*)(int, string);
	fptr f1 = &MyClass12::hello;
	//类的成员指针
	using ptr1 = int MyClass12::*;
	ptr1 f2 = &MyClass12::id;

	/*MyClass12 m1;
	(m1.*f1)(12, "111");
	MyClass12 m2;
	(m2.*f2) = 10;
	cout << "id=" << m2.id << endl;*/
}

//13可调用对象包装器function的使用
void fun13_1(int a, string b) { cout << a << " " << b << endl; }
void test13() {
	//①包装普通函数
	function<void(int, string)>f1 = fun13_1;
	//②包装类的静态函数
	function<void(int, string)>f2 = MyClass12::world;
	//③包装仿函数
	MyClass12 m12_1;
	function<void(string)>f3 = m12_1;
	//④包装转化为函数指针的对象
	MyClass12 m12_2;
	function<void(int, string)>f4 = m12_2;
	//调用
	f1(1, "aaa");
	f2(2, "bbb");
	f3("ccc");
	f4(4, "ddd");
}

class MyClass13 {
public:
	MyClass13(const function<void(int, string)>& f) :callback(f) {}

private:
	function<void(int, string)>callback;
};

//14.移动构造函数

//15.智能指针
//智能指针的初始化
class MyClass15 {
public:
	int data;
	MyClass15(int val) : data(val) {
		cout << "MyClass15 constructor called. data = " << data << endl;
	}
	MyClass15(int a, int b, int c) {
		data = a + b + c;
		cout << "int int int" << endl;
	}
	//智能指针会自动调用析构函数
	~MyClass15() {
		cout << "MyClass15 destructor called. data = " << data << endl;
	}
	void print15() {
		cout << "call print15()" << endl;
	}
};

template<class T>
void fun15_delete(T* p) {
	delete p;
	cout << "指针p被释放了" << endl;
}

void fun15_shared() {
	//一、调用构造函数
	shared_ptr<int>ptr1(new int(1));//1
	cout << "ptr1管理的内存引用计数" << ptr1.use_count() << endl;
	//二、调用拷贝构造函数
	cout << "拷贝构造函数" << endl;
	shared_ptr<int>ptr2 = ptr1;//2
	cout << "ptr2管理的内存引用计数" << ptr2.use_count() << endl;
	//调用移动构造函数
	shared_ptr<int>ptr3 = move(ptr2);//2 后续不能使用ptr2
	cout << "ptr3管理的内存引用计数" << ptr3.use_count() << endl;//use_count还是2 因为移动构造的本质是把ptr2所有权交给了ptr3
	/*if (!ptr2)
		cout << nullptr;*/  //ptr2为空

		//三、reset函数(不能修改原来指针的类型)
	cout << "利用reset函数" << endl;
	ptr1.reset();//指针充值
	ptr1.reset(new int(0));//ptr1不再管理原来的内存
	//ptr1.reset(new double(0));//error ptr1由指向int变为指向double reset不能做到   ptr1不再管理原来的内存
	cout << "ptr1调用reset后ptr3管理的内存引用计数" << ptr3.use_count() << endl;//1

	//四、make_shared
	cout << "利用make_shared模版函数" << endl;
	// ①可以使用 make_shared 创建一个空的 shared_ptr
	shared_ptr<int> ptr4;
	ptr4 = make_shared<int>(100);
	cout << "ptr4's data: " << *ptr4 << endl;
	// ②使用 make_shared 初始化一个 shared_ptr
	shared_ptr<MyClass15> ptr5 = make_shared<MyClass15>(42);
	cout << "ptr5's data: " << ptr5->data << endl;
	// ③对于自定义构造函数的类，可以传递多个参数
	shared_ptr<MyClass15> ptr6 = make_shared<MyClass15>(1, 2, 3);
	cout << "ptr6's data: " << ptr6->data << endl;
	// ④可以使用 auto 关键字简化 shared_ptr 的声明
	auto ptr7 = make_shared<MyClass15>(200);
	cout << "ptr7's data: " << ptr7->data << endl;

	//五、原始指针的获取
	cout << "原始指针的获取:" << endl;
	//获取原始指针
	MyClass15* p = ptr7.get();
	p->print15();
	ptr7->print15();
	//定义释放函数
	shared_ptr<int>ptr8(new int(0), fun15_delete<int>);
}

//16.lambda表达式
//语法[capture](params)opt->ret{body;}     [捕捉方式](参数列表)(mutable/exception)返回值后置 可自动推导{函数体}
void lambdaExamples() {
	// 1. 不捕获任何外部变量
	auto lambda1 = []() {
		cout << "This lambda captures nothing." << endl;
		};
	lambda1();
	//也可以这么调用 不需要lambda()
	/*[]() {
		cout << "This lambda captures nothing." << endl;
		}();*/

		// 2. 按值捕获外部变量
	int x = 5;
	auto lambda2 = [x]() {
		cout << "This lambda captures x by value: " << x << endl;
		};
	lambda2();

	// 3. 按引用捕获外部变量
	int y = 10;
	auto lambda3 = [&y]() {
		y++;  // 修改引用捕获的变量
		cout << "This lambda captures y by reference: " << y << endl;
		};
	lambda3();
	cout << "y after lambda3: " << y << endl;

	// 4. 混合捕获，按值捕获 x，按引用捕获 y
	auto lambda4 = [x, &y]() {
		cout << "This lambda captures x by value: " << x << " and y by reference: " << y << endl;
		y++;  // 可以修改 y
		};
	lambda4();
	cout << "y after lambda4: " << y << endl;

	// 5. 捕获列表中的默认捕获
	int z = 15;
	auto lambda5 = [=]() {  // 按值捕获所有外部变量 只读
		cout << "This lambda captures all variables by value: x=" << x << ", y=" << y << ", z=" << z << endl;
		};
	lambda5();

	// 6. 捕获列表中的默认引用捕获
	auto lambda6 = [&]() {  // 按引用捕获所有外部变量
		cout << "This lambda captures all variables by reference: x=" << x << ", y=" << y << ", z=" << z << endl;
		y++;  // 可以修改 y 和 z
		z++;
		};
	lambda6();
	cout << "y after lambda6: " << y << ", z after lambda6: " << z << endl;

	// 7. 捕获 this 指针（在类的成员函数中使用）
	// 只能使用类成员对象
	// 假设存在一个类 MyClass 及其成员函数
	/*
	class MyClass {
	public:
		void memberFunction() {
			int a = 20;
			auto lambda7 = [this]() {
				cout << "This lambda captures this pointer. a=" << a << endl;
			};
			lambda7();
		}
	private:
		int a;
	};
	*/

	// 8. 捕获列表中的初始化捕获（C++14 及以后）
	int w = 25;
	auto lambda8 = [captured_w = w * 2]() {
		cout << "This lambda captures captured_w initialized by w * 2: " << captured_w << endl;
		};
	lambda8();

	// 9. mutable 关键字，允许修改按值捕获的变量
	int m = 30;
	auto lambda9 = [m]() mutable {
		m++;  // 由于使用了 mutable，可以修改按值捕获的 m
		cout << "This lambda captures m by value and modifies it using mutable: " << m << endl;
		};
	lambda9();
	cout << "m after lambda9: " << m << endl;

	// 10. 带有异常说明的 lambda 表达式
	auto lambda10 = []() noexcept {
		cout << "This lambda is declared as noexcept." << endl;
		};
	lambda10();

	// 11. 带有返回值的 lambda 表达式
	auto lambda11 = []() -> int {
		return 42;
		};
	int result = lambda11();
	cout << "lambda11 returns: " << result << endl;

	// 12. 完整的 lambda 表达式，包含参数列表、捕获列表、异常说明和返回值
	auto lambda12 = [x, &y](int a, int b) noexcept -> int {
		y++;
		return x + a + b;
		};
	int sum = lambda12(5, 10);
	cout << "lambda12 returns: " << sum << ", y after lambda12: " << y << endl;
}
#if 0
//17bind
// 普通函数
int add(int a, int b) {
	return a + b;
}
class Calculator {
public:
	int multiply(int a, int b) {
		return a * b;
	}
};
void handleClick(int id) {
	std::cout << "Button " << id << " clicked!" << std::endl;
}

class Button {
public:
	using ClickCallback = std::function<void()>;

	Button(int id, ClickCallback callback) : id_(id), callback_(callback) {}

	void simulateClick() {
		callback_();
	}

private:
	int id_;
	ClickCallback callback_;
};

void test17_() {
	std::vector<Button> buttons;
	// 创建按钮并绑定点击事件处理函数
	for (int i = 0; i < 3; ++i) {
		auto clickHandler = std::bind(handleClick, i);
		buttons.emplace_back(i, clickHandler);
	}
	// 模拟按钮点击
	for (auto& button : buttons) {
		button.simulateClick();
	}
}
void test17() {
	// 完全绑定参数
	auto add5and3 = std::bind(add, 5, 3);
	std::cout << "add5and3(): " << add5and3() << std::endl;

	// 部分绑定参数
	auto addTo5 = std::bind(add, 5, std::placeholders::_1);
	std::cout << "addTo5(10): " << addTo5(10) << std::endl;

	Calculator calc;
	// 绑定成员函数
	auto multiplyBy2 = std::bind(&Calculator::multiply, &calc, 2, std::placeholders::_1);
	std::cout << "multiplyBy2(5): " << multiplyBy2(5) << std::endl;
}
#endif

//18.右值引用 利用右值初始化右值引用

class MyClass18 {
public:
	// 默认构造函数，初始化指针成员
	MyClass18() : m_num(new int(0)) {
		std::cout << "Default constructor called." << std::endl;
	}

	// 移动构造函数，实现浅拷贝
	MyClass18(MyClass18&& a) noexcept : m_num(a.m_num) {
		a.m_num = nullptr;
		std::cout << "Move constructor called." << std::endl;
	}

	// 析构函数，释放动态分配的内存
	~MyClass18() {
		delete m_num;
	}

	// 返回一个 MyClass18 类型的临时对象
	MyClass18 getObj18() {
		return MyClass18();
	}

private:
	int* m_num;
};

void test18() {
	// 左值
	int num = 9;
	// 左值引用
	int& a = num;
	// 右值
	// 右值引用(右值引用是一个左值)
	int&& b = 1;
	// 常量左值引用
	const int& c = num;
	// 常量右值引用
	const int&& d = 6;
	// const int &&d = b; // error(右值引用是一个左值)
	// 右值引用是一个左值
	const int& e = b;
	int& g = b;

	MyClass18 f;
	// 右侧必须是一个临时对象 才会调用移动构造函数
	MyClass18&& t = f.getObj18();
}
//19.move forward
//move
#if 0
int main() {
	std::vector<std::string> vec;
	std::string str = "Hello, World!";
	// 使用 std::move 将 str 转换为右值引用，调用移动构造函数
	vec.push_back(std::move(str));
	// 此时 str 处于有效但未指定的状态
	std::cout << "vec[0]: " << vec[0] << std::endl;
	return 0;
}

class MyClass {
public:
	MyClass() : data(new int[100]) {
		std::cout << "Constructor" << std::endl;
	}

	// 移动构造函数
	MyClass(MyClass&& other) noexcept : data(std::move(other.data)) {
		other.data = nullptr;
		std::cout << "Move Constructor" << std::endl;
	}

	~MyClass() {
		delete[] data;
	}

private:
	int* data;
};

int main() {
	MyClass obj1;
	MyClass obj2 = std::move(obj1);
	return 0;
}
//forward
// 目标函数
void foo(int& x) {
	std::cout << "Lvalue reference: " << x << std::endl;
}

void foo(int&& x) {
	std::cout << "Rvalue reference: " << x << std::endl;
}

// 转发函数模板
template<typename T>
void forward_foo(T&& arg) {
	foo(std::forward<T>(arg));
}

int main() {
	int a = 10;
	forward_foo(a); // 传递左值
	forward_foo(20); // 传递右值
	return 0;
}
#endif

#if 0
//20typename... Args
#include <iostream>

// 终止递归的函数
void print() {
	std::cout << std::endl;
}

// 可变参数模板函数
template<typename T, typename... Args>
void print(T first, Args... args) {
	std::cout << first;
	//c++17
	if constexpr (sizeof...(args) > 0) {
		std::cout << ", ";
	}
	print(args...); // 递归调用
}

int main() {
	print(1, 2.5, "hello");
	return 0;
}

#endif
#if 0
int main() {
	/*{test03_3<test03_T1>();
	test03_3<test03_T2>();
	}*/
	//test04();
	/*{
		vector<int>v1 = { 1,2,3 };
		Container<vector<int>>con;
		con.__4printSingle(v1);
		const vector<int>v2 = { 1,2,3 };
		Container<const vector<int>>con2;
		con2.__4printSingle(v2);
	}*/
	//test05();
	//test06();
	//Derived().callBaseFunc();
	//test07();
	//test09();
	//test10();
	//test11();
	//fun13_shared();
	//test12();
	test13();

	//进程45集  lambda中的function
}

#endif