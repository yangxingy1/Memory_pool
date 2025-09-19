#pragma once

#include<atomic>
#include<cstdint>
#include<iostream>
#include<memory>
#include<mutex>

namespace MemoryPool
{
#define MEMORY_POOL_CAPACITY 64;
#define SLOT_BASE_SIZE 8;
#define MAX_SLOT_SIZE 512;

struct Slot
{
    std::atomic<Slot*> next; // 原子指针
};

class Memory_pool
{
private:
    int BlockSize;                  // 内存块大小
    int SlotSize;                   // 槽大小
    Slot* firstSlot;                // 第一个槽
    Slot* curSlot;                  // 第一个闲
    Slot* lastSlot;                 // 最后一个槽   超过要再申请内存
    std::atomic<Slot*> freeList;    // 被使用过后又被释放的槽
    std::mutex mutex;               // 锁


private:
    void allocNewBlock();
    size_t padPointer(char* p, size_t align);

    bool pushFreeList(Slot* slot);
    Slot* popFreeList();

public:
    Memory_pool(size_t BlockSize=4096);
    ~Memory_pool();

    void init(size_t);

    void* allocate();
    void deallocate(void*);


};
}
