/*饿汉*/

#include <iostream>
#include <mutex>

using namespace std;

class Single {
private:
  Single() = default;
  Single(const Single &) = delete;
  Single &operator=(const Single &) = delete;
  static Single *single;
  static std::mutex m_mutex; // 必须使用static代表同一把锁 而不是多个锁

public:
  ~Single() {
    delete single;
    std::cout << "destruct" << std::endl;
  }

  static Single *getInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!single) {
      /*并发调用 可能存在线程安全问题 除非线程创建之前就调用或者加锁*/
      single = new Single();
    }
    return single;
  }
};
/*静态变量必须先定义*/
Single *Single::single = Single::getInstance();           
std::mutex Single:: m_mutex;

int main() {
  /*创建线程开始之前就创建对象*/
  Single *ptr = Single::getInstance();
  std::cout << 1 << std::endl;
  return 0;
}