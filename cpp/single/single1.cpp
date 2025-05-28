#include <iostream>
#include <memory.h>

class Single
{  
private:
    Single() = default;
    Single(const Single &) = delete;
    Single &operator=(const Single &) = delete;
public:
    ~Single()
    {
        std::cout << "destruct" << std::endl;
    }
    static Single &getInstance()
    {
        static Single single;
        return single;
    }
};