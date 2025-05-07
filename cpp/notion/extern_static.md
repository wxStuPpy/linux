在 C 和 C++ 中，`extern` 和 `static` 是两个重要的关键字，它们在变量和函数的声明与定义中有着不同的作用，下面分别介绍它们的具体用法。

### `extern` 关键字
#### 1. 用于变量声明
当你在一个文件中需要使用另一个文件中定义的全局变量时，可以使用 `extern` 关键字进行声明。`extern` 声明不会为变量分配内存，它只是告诉编译器该变量在其他地方已经定义过了。

**示例**：
假设我们有两个文件 `file1.c` 和 `file2.c`。

`file1.c` 文件内容：
```c
#include <stdio.h>

// 定义一个全局变量
int globalVariable = 10;

void printVariable();

int main() {
    printVariable();
    return 0;
}
```

`file2.c` 文件内容：
```c
#include <stdio.h>

// 使用 extern 声明全局变量
extern int globalVariable;

void printVariable() {
    printf("The value of globalVariable is: %d\n", globalVariable);
}
```
在 `file2.c` 中，`extern int globalVariable;` 声明了 `globalVariable` 是一个外部定义的全局变量，这样 `printVariable` 函数就可以使用这个变量了。

#### 2. 用于函数声明
在 C 和 C++ 中，函数的声明默认就是 `extern` 的，所以通常不需要显式地使用 `extern` 来声明函数。不过，显式使用 `extern` 可以更清晰地表明该函数是在其他文件中定义的。

**示例**：
```c
// 显式使用 extern 声明函数
extern void function();

int main() {
    function();
    return 0;
}
```

### `static` 关键字
#### 1. 用于局部变量
当 `static` 用于修饰局部变量时，该变量会在程序的整个生命周期内存在，而不是像普通局部变量那样在函数调用结束后就被销毁。`static` 局部变量只会被初始化一次，之后每次调用该函数时，它都会保留上一次调用结束时的值。

**示例**：
```c
#include <stdio.h>

void increment() {
    // 定义一个 static 局部变量
    static int count = 0;
    count++;
    printf("Count: %d\n", count);
}

int main() {
    increment();
    increment();
    increment();
    return 0;
}
```
在这个例子中，`count` 是一个 `static` 局部变量，每次调用 `increment` 函数时，它的值都会保留并递增。

#### 2. 用于全局变量和函数
当 `static` 用于修饰全局变量或函数时，它会限制这些变量和函数的作用域仅限于定义它们的文件。也就是说，这些变量和函数不能被其他文件访问，起到了信息隐藏的作用。

**示例**：
```c
// file1.c
#include <stdio.h>

// 定义一个 static 全局变量
static int staticGlobalVariable = 20;

// 定义一个 static 函数
static void staticFunction() {
    printf("This is a static function.\n");
}

int main() {
    staticFunction();
    printf("The value of staticGlobalVariable is: %d\n", staticGlobalVariable);
    return 0;
}
```
在这个例子中，`staticGlobalVariable` 和 `staticFunction` 只能在 `file1.c` 文件中使用，其他文件无法访问它们。

### 总结
- `extern` 主要用于声明外部定义的变量和函数，让编译器知道这些变量和函数在其他地方已经定义过。
- `static` 用于局部变量时，延长其生命周期；用于全局变量和函数时，限制其作用域为定义它们的文件。 