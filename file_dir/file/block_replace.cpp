#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <climits>
using namespace std;

// FIFO页面置换算法
double fifo(const vector<int> &pages, int memorySize)
{
    queue<int> q;
    unordered_set<int> memory;
    // 缺页次数
    int pageFaults = 0;

    // 遍历页面序列
    for (int current : pages)
    {
        // 如果当前页面在内存中，跳过本次循环
        if (memory.count(current))
            continue;

        pageFaults++;
        // 如果内存未满，将当前页面加入内存和队列
        if (memory.size() < memorySize)
        {
            memory.insert(current);
            q.push(current);
        }
        else
        {
            // 内存已满，使用FIFO策略，移除队列头部的页面（即最早进入内存的页面）
            int removed = q.front();
            q.pop();
            memory.erase(removed);
            memory.insert(current);
            q.push(current);
        }
    }
    return (pages.size() - pageFaults) * 100.0 / pages.size();
}

// LRU页面置换算法
double lru(const vector<int>& pages, int memorySize) {
    // 用于存储页面及其最后使用时间的哈希表
    unordered_map<int, int> lastUsed;
    int time = 0, pageFaults = 0;
    for (int current : pages) {
        time++;
        if (lastUsed.count(current)) {
            // 如果在内存中，更新该页面的最后使用时间
            lastUsed[current] = time;
            // 命中，跳过本次循环
            continue;
        }
        pageFaults++;
        // 内存未满，将当前页面加入内存，并记录其最后使用时间
        if (lastUsed.size() < memorySize) {
            lastUsed[current] = time;
        } else {
            int lruTime = INT_MAX, lruPage = -1;
            // 遍历哈希表，找到最久未使用的页面
            for (auto& it : lastUsed) {
                // 如果当前页面的最后使用时间小于lruTime
                if (it.second < lruTime) {
                    lruTime = it.second;
                    lruPage = it.first;
                }
            }
            // 移除最久未使用的页面（从哈希表中删除）
            lastUsed.erase(lruPage);
            // 将当前页面加入内存，并记录其最后使用时间
            lastUsed[current] = time;
        }
    }
    return (pages.size() - pageFaults) * 100.0 / pages.size();
}
// OPT页面置换算法 找到后面最久为使用的页面
double opt(const vector<int> &pages, int memorySize)
{
    vector<int> memory;
    int pageFaults = 0;

    for (size_t i = 0; i < pages.size(); ++i)
    {
        int current = pages[i];
        bool found = false;

        for (int p : memory)
        {
            if (p == current)
            {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        pageFaults++;

        if (memory.size() < memorySize)
        {
            memory.push_back(current);
        }
        else
        {
            int farthest = -1, replaceIdx = 0;

            // 找到未来最远使用的页面，遍历当前内存中的页面
            for (size_t j = 0; j < memory.size(); ++j)
            {
                int nextUse = INT_MAX;
                // 从当前位置往后遍历页面序列，找到下一次使用该页面的位置
                for (size_t k = i + 1; k < pages.size(); ++k)
                {
                    if (pages[k] == memory[j])
                    {
                        nextUse = k; // 记录下一次使用该页面的位置
                        break;
                    }
                }
                // 如果下一次使用该页面的位置大于当前记录的最远位置
                if (nextUse > farthest)
                {
                    farthest = nextUse; // 更新最远位置
                    replaceIdx = j;     // 更新要替换的页面在内存中的索引
                }
            }
            // 用当前页面替换未来最远使用的页面
            memory[replaceIdx] = current;
        }
    }
    return (pages.size() - pageFaults) * 100.0 / pages.size();
}

int main(int argc, char *argv[])
{
    vector<int> pages(50);
    // 尝试打开命令行参数指定的文件
    ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        cerr << "无法打开文件 " << argv[1] << endl;
        return 1;
    }

    // 从文件中读取50个页面号
    for (int i = 0; i < 50; ++i)
    {
        if (!(inputFile >> pages[i]))
        {
            cerr << "读取文件时出错, 数据不足50个" << endl;
            return 1;
        }
    }
    inputFile.close();

    // 输出页面访问顺序
    cout << "===== 页面访问顺序 =====" << endl;
    for (int i = 0; i < 50; ++i)
    {
        cout << pages[i] << " ";
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    cout << endl;

    const int memorySize = 4;

    // 计算并输出三种算法的命中率
    cout << "===== 页面置换算法命中率 =====" << endl;
    cout << "FIFO 命中率: " << fifo(pages, memorySize) << "%" << endl;
    cout << "LRU 命中率: " << lru(pages, memorySize) << "%" << endl;
    cout << "OPT 命中率: " << opt(pages, memorySize) << "%" << endl;

    return 0;
}