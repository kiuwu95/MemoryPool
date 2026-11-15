#include<iostream>
#include<vector>
#include<cassert>
#include<chrono>
#include<utility>

class MemoryPool {
private:
    //将小块内存大小设定为能运行的正常值
    size_t adjust_block_size(const size_t& size);
    //每次向系统申请一页内存，并把这一页内存的切成很多小块并把它们挂到空闲链表中
    void expand();
public:
    //分配一小块内存
    void* allocate();
    //回收内存，归还小块内存，插入链表头部
    void deallocate(void* p);

    size_t BlocksSize() { return blocks_size; }
    size_t BlockPagesSize() { return blocks_per_page; }

    //析构时释放内存页
    ~MemoryPool();
    explicit MemoryPool(const size_t& block_size, const size_t& block_per_page = 1024);
private:
    size_t blocks_size;//小块内存的大小
    size_t blocks_per_page;//一页内存有多少块小块内存
    std::vector<void*> m_pages;//页的vector
    //空闲小块内存链表
    struct Node {
        Node* next;
    };
    Node* free_list;
};