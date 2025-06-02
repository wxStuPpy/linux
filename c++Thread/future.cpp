#include<future>
#include<mutex>
#include<thread>
#include<iostream>

using namespace std;

void deadLock(){
    std::mutex m;
    std::cout<<"begin"<<endl;
    std::lock_guard<std::mutex> lk1(m);
    {
        //异步返回的future对象析构时会阻塞等待future对象完成，此处会死锁
        std::future<int>f=std::async(launch::async, [&m]{
            std::this_thread::sleep_for(chrono::seconds(1));
            std::cout<<"deadLock"<<endl;
            std::lock_guard<std::mutex> lk2(m);
            std::cout<<"endLock"<<endl;
            return 1;
        });
    }
    std::cout<<"Done"<<endl;
}

void SafeVersion() {
    std::mutex mtx;
    std::cout << "Begin" << std::endl;
    std::future<void> futures; // 声明在外部作用域

    {
        std::lock_guard<std::mutex> dklock(mtx);
        futures = std::async(std::launch::async, [&mtx]() {
            std::lock_guard<std::mutex> dklock(mtx);
            std::cout << "async working" << std::endl;
        });
    } // 锁在此释放，但future仍存在

    futures.get(); // 等待任务完成
    std::cout << "End" << std::endl;
}

int main(){
    //deadLock();
    SafeVersion();
}
