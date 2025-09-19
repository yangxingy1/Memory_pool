#include"../include/MemoryPool.h"

namespace MemoryPool
{

// 计算内存对齐所需的地址数
size_t Memory_pool::padPointer(char* p, size_t slot_size)
{
    return slot_size - (reinterpret_cast<size_t>(p) % slot_size);
}

// 这里申请一个大的内存块 把内存块分为 -> 1.指向下一个内存块的指针      2.多个内存槽(Slot)
void Memory_pool::allocNewBlock()
{
    void* newBlock = operator new(BlockSize);
    // 头插法在大的内存块头部放入指向下一个内存块的指针
    // 由于每个内存块头部都有一个struct Slot结构体原子指针 因此可以用Slot* 指针代表内存块
    reinterpret_cast<Slot*>(newBlock)->next = firstSlot;
    firstSlot = reinterpret_cast<Slot*>(newBlock);

    // 越过指针部分 计算可用的内存槽
    // -- 先进行内存对齐 找到整数倍的内存地址 -> 强化内存处理性能 --
    // -- 再切割内存块 划分出内存槽 --
    char* body = reinterpret_cast<char*>(newBlock) + sizeof(Slot*);
    size_t paddingSize = padPointer(body, SlotSize);            // 需要填充的内存大小
    curSlot = reinterpret_cast<Slot*>(body + paddingSize);      // 当前位置

    // 计算出最后一个槽的首地址 -> 作为申请下一块Block的阈值
    lastSlot = reinterpret_cast<Slot*>(reinterpret_cast<size_t>(newBlock) + BlockSize - SlotSize);
}




Memory_pool::Memory_pool(size_t BlockSize)
    : BlockSize(BlockSize)
    , SlotSize(0)
    , firstSlot(nullptr)
    , curSlot(nullptr)
    , lastSlot(nullptr)
    , freeList(nullptr)
{}

Memory_pool::~Memory_pool()
{
    Slot* p=firstSlot;
    while(p)
    {
        Slot* next = p->next;
        // free的对等函数 强转成 void* 
        operator delete(reinterpret_cast<void*>(p));
        p = next;
    }
}

void Memory_pool::init(size_t size)
{
    if(size <= 0)
        return;
    SlotSize = size;
    firstSlot = nullptr;
    curSlot = nullptr;
    freeList = nullptr;
    lastSlot = nullptr;
}

void* Memory_pool::allocate()
{
    // 优先使用空闲列表中的内存块 -> 已经被使用过但释放掉的内存块

}


}