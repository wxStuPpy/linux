#include<func.h>
/// @brief 
/// @param key 0或者IPC_PRIVATE代表私密 仅支持父子间 其他正数作为通信标识
/// @param size 页大小(4096)的整数倍
/// @param shmflg 权限
/// @return 物理内存的标识 需要shmat映射到虚拟内存之后才可使用
// int shmget(key_t key, size_t size, int shmflg);

int main(void){
    //在物理内存创建共享内存
    int shmid=shmget(IPC_PRIVATE,4096,IPC_CREAT|0600);
    //将物理内存映射到虚拟内存上
    /*char *p=(char*)shmat(shmid,nullptr,0);nullptr代表自动分配内存
    memcpy(p,"hello",5);
    printf("%s\n",p);*/
    int *p = (int *)(shmat(shmid, nullptr, 0));
    *p=0;
    if(fork()){
        for(int i=0;i<10;++i){
            ++*p;
        }
        wait(nullptr);
        printf("*p = %d\n",*p);//存在竞争
    }
    else{
        for(int i=0;i<10;++i){
            ++*p;
        }
    }

    //把共享内存段从当前进程的地址空间分离
    shmdt(p);
    return 0;
    /*删除正在使用的共享内存是 暂时会使key==0 status==dest
    不允许其他新的进程连接该进程 所有相关进程运行完后销毁*/
}