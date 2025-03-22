#pragma once

#include <algorithm>
#include <vector>

// BinaryHeap aka PriorityQueue
template <typename Tp_, class Compare_ = std::less<Tp_>>
class Heap {
public:
    Heap();

    void Push(Tp_ value);
    void Pop();
    Tp_& Top();
    size_t Size();

private:
    void SiftUp(size_t v);
    void SiftDown(size_t v);

    std::vector<Tp_> container_;
    size_t size_;
    Compare_ comp_;
};

template <typename Tp_, class Compare_>
Heap<Tp_, Compare_>::Heap() : size_(1) {
    container_.resize(1);
}

template <typename Tp_, class Compare_>
void Heap<Tp_, Compare_>::Push(Tp_ value) {
    container_.push_back(value);
    ++size_;
    SiftUp(size_ - 1);
}

template <typename Tp_, class Compare_>
void Heap<Tp_, Compare_>::Pop() {
    std::swap(container_[1], container_[size_ - 1]);
    container_.pop_back();
    --size_;
    SiftDown(1);
}

template <typename Tp_, class Compare_>
Tp_& Heap<Tp_, Compare_>::Top() {
    return container_[1];
}

template <typename Tp_, class Compare_>
size_t Heap<Tp_, Compare_>::Size() {
    return size_ - 1;
}

template <typename Tp_, class Compare_>
void Heap<Tp_, Compare_>::SiftUp(size_t v) {
    while (v > 1 && comp_(container_[v >> 1], container_[v])) {
        swap(container_[v], container_[v >> 1]);
        v >>= 1;
    }
}

template <typename Tp_, class Compare_>
void Heap<Tp_, Compare_>::SiftDown(size_t v) {
    while ((v << 1) < size_) {
        size_t l = v << 1;
        size_t r = v << 1 | 1;
        size_t u = (r < size_ && comp_(container_[l], container_[r]) ? r : l);

        if (comp_(container_[v], container_[u])) {
            swap(container_[v], container_[u]);
            v = u;
        } else {
            break;
        }
    }
}
