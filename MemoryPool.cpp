#include"MemoryPool.h"

//算法
static inline std::size_t align_up(size_t n, size_t align) {
    return (n + (align - 1)) & ~(align - 1);
}
//将小块内存大小设定为能运行的正常值
size_t MemoryPool::adjust_block_size(const size_t& size) {
    if (size < sizeof(void*)) {
        return align_up(sizeof(void*), alignof(void*));
    }
    return align_up(size, alignof(void*));
}
//每次向系统申请一页内存，并把这一页内存的切成很多小块并把它们挂到空闲链表中
void MemoryPool::expand() {
    size_t pages_bytes = blocks_per_page * blocks_size;
    char* p = static_cast<char*>(::operator new[](pages_bytes));
    m_pages.push_back(p);

    for (int i = 0;i < blocks_per_page;i++) {
        char* add_blk = p + i * blocks_size;
        Node* add_node = reinterpret_cast<Node*>(add_blk);

        add_node->next = free_list;
        free_list = add_node;
    }
}
//分配一小块内存
void* MemoryPool::allocate() {
    if (!free_list) {
        expand();
    }
    Node* head = free_list;
    free_list = head->next;
    return head;
}
//回收内存，归还小块内存，插入链表头部
void MemoryPool::deallocate(void* p) {
    if (!p)return;

    Node* head = static_cast<Node*>(p);
    head->next = free_list;
    free_list = head;
}
MemoryPool::~MemoryPool() {
    for (auto& it : m_pages) {
        ::operator delete[](it);
    }
}
explicit MemoryPool::MemoryPool(const size_t& block_size, const size_t& block_per_page = 1024) :blocks_per_page(block_per_page),
free_list(nullptr), blocks_size(adjust_block_size(block_size)) {
}