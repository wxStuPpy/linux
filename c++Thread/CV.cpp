#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>

using namespace std;

int flag=0;
mutex mtx;
condition_variable cv;
bool running = true;  // 控制循环终止的标志

int main(){
    thread t1([](){
        while(running) {  // 添加循环
            unique_lock<mutex> lck(mtx);
            cv.wait(lck, []{return flag==0;});
            if(!running) break;  // 检查是否需要终止
            cout<<"t1"<<endl;
            flag=1;
            lck.unlock();
            cv.notify_one();
            this_thread::sleep_for(chrono::seconds(1));
        }
    });

    thread t2([](){
        while(running) {  // 添加循环
            unique_lock<mutex> lck(mtx);
            cv.wait(lck, []{return flag==1;});
            if(!running) break;  // 检查是否需要终止
            cout<<"t2"<<endl;
            flag=0;
            lck.unlock();
            cv.notify_one();
            this_thread::sleep_for(chrono::seconds(1));
        }
    });
    
    // 主线程等待一段时间后终止循环
    this_thread::sleep_for(chrono::seconds(10));
    {
        lock_guard<mutex> lck(mtx);
        running = false;  
        flag = 0;         
    }
    cv.notify_all();  
    
    t1.join();
    t2.join();
}