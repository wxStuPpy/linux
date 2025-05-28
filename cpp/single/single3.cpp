/*懒汉 需要时创建*/

#include <iostream>
#include <mutex>

using namespace std;

class Single {
private:
  Single() = default;
  Single(const Single &) = delete;
  Single &operator=(const Single &) = delete;
  static Single *single;
static std::mutex m_mutex;

public:
  ~Single() {
    delete single;
    std::cout << "destruct" << std::endl;
  }
  static Single *getInstance() {
    if(!single){
          
    }

    if(!single){

    }
  }
};

Single *Single::single = Single::getInstance();

int main() {
  Single *ptr = Single::getInstance();
  std::cout << 1 << std::endl;
  return 0;
}