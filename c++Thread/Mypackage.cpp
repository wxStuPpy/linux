#include<mutex>
#include<iostream>
#include<condition_variable>
#include<future>
#include<thread>
using namespace std;

int myTask(int x){
    //模拟耗时操作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Task " << x << endl;
    return x;
}

void usePackage(){
    packaged_task<int(int)> task1(myTask);
    future<int> f1 = task1.get_future();
    thread t1(std::move(task1),2);

    cout << "Package Doing something" << endl;
    cout << "Result: " << f1.get() << endl;
    t1.join();
}

void setValue(promise<int>&p,int x){
    this_thread::sleep_for(std::chrono::seconds(2));
    p.set_value(x);
    cout << "Promise set value" << endl;
    return;
}

void usePromise(){
    promise<int> p;
    auto f1 = p.get_future();
    thread t1(setValue,std::ref(p),2);
    cout << "Promise Doing something" << endl;
    cout << "Promise Result: " << f1.get() << endl;
    t1.join();
}

int main(){
    future<int> f1 = async(launch::async, myTask, 1);
    cout << "Doing something" << endl;
    cout << "Result: " << f1.get() << endl;

    usePackage();
    usePromise();
    cout << "Done" << endl;
    return 0;
}