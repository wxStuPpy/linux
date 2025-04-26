以下是三种典型死锁场景的分析与解决方案，结合代码示例说明：

---

### **1. 两个线程竞争两把锁（顺序不一致）**
#### **场景描述**
线程1先锁A再锁B，线程2先锁B再锁A，形成循环等待。
```c
pthread_mutex_t lockA, lockB;

void* thread1(void*) {
    pthread_mutex_lock(&lockA);
    sleep(1);  // 模拟耗时操作
    pthread_mutex_lock(&lockB);  // 阻塞（lockB被thread2持有）
    // ...
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
}

void* thread2(void*) {
    pthread_mutex_lock(&lockB);
    sleep(1);
    pthread_mutex_lock(&lockA);  // 阻塞（lockA被thread1持有）
    // ...
    pthread_mutex_unlock(&lockA);
    pthread_mutex_unlock(&lockB);
}
```

#### **解决方案**
- **固定上锁顺序**：所有线程按相同顺序获取锁（如先A后B）。
- **超时机制**：使用 `pthread_mutex_trylock` 或带超时的锁。

---

### **2. 上锁线程异常中止（未释放锁）**
#### **场景描述**
线程加锁后崩溃或未解锁，导致其他线程永久阻塞。
```c
void* thread_func(void*) {
    pthread_mutex_lock(&lockA);
    int* ptr = NULL;
    *ptr = 42;  // 触发段错误，线程崩溃，锁未释放！
    pthread_mutex_unlock(&lockA);
}
```

#### **解决方案**
- **锁的健壮性属性**：
  ```c
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);  // 设置健壮锁
  pthread_mutex_init(&lockA, &attr);
  ```
- **恢复机制**：
  ```c
  if (pthread_mutex_lock(&lockA) == EOWNERDEAD) {
      // 前任线程死亡，需清理状态并标记锁可恢复
      pthread_mutex_consistent(&lockA);
  }
  ```

---

### **3. 对已上锁的锁重复上锁（非递归锁）**
#### **场景描述**
同一线程对非递归锁多次加锁，导致自身死锁。
```c
void* thread_func(void*) {
    pthread_mutex_lock(&lockA);
    pthread_mutex_lock(&lockA);  // 第二次加锁阻塞（非递归锁）
    // ...
    pthread_mutex_unlock(&lockA);
}
```

#### **解决方案**
- **使用递归锁（可重入锁）**：
  ```c
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);  // 设置为递归锁
  pthread_mutex_init(&lockA, &attr);
  ```
- **避免重复加锁**：检查锁状态或重构代码逻辑。

---

### **综合对比表**
| **死锁类型**               | **触发条件**                     | **解决方案**                              |
|----------------------------|----------------------------------|------------------------------------------|
| 两线程竞争两把锁（顺序相反） | 线程1: A→B，线程2: B→A           | 固定上锁顺序；超时机制                   |
| 线程异常中止未释放锁        | 加锁后线程崩溃或未解锁           | 健壮锁（`PTHREAD_MUTEX_ROBUST`）         |
| 重复上锁（非递归锁）        | 同一线程对非递归锁多次加锁       | 改用递归锁；避免重复加锁                 |

---

### **最佳实践建议**
1. **锁顺序规则**：全局约定锁的获取顺序（如按地址排序）。
2. **超时机制**：
   ```c
   struct timespec timeout;
   clock_gettime(CLOCK_REALTIME, &timeout);
   timeout.tv_sec += 1;  // 设置1秒超时
   if (pthread_mutex_timedlock(&lockA, &timeout) != 0) {
       // 处理超时
   }
   ```
3. **RAII模式**：封装锁的获取/释放到对象生命周期中（C++适用）。
4. **静态分析工具**：使用 `helgrind` 或 `clang-thread-safety` 检测潜在死锁。