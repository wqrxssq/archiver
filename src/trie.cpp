#include "archiver.h"

// public
//////////////////////////////////////////////////////////////////////////////////////////
Trie::Trie() : parent_(nullptr), left_son_(nullptr), right_son_(nullptr), value_(0) {
}
Trie::Trie(char16_t c) : parent_(nullptr), left_son_(nullptr), right_son_(nullptr), value_(c) {
}

Trie::Trie(Trie* left, Trie* right) {
    left_son_ = left;
    right_son_ = right;
    value_ = std::min(right->value_, left->value_);
    left->parent_ = this;
    right->parent_ = this;
    parent_ = nullptr;
}

void Trie::CleanTrie() {
    if (left_son_ != nullptr) {
        left_son_->CleanTrie();
    }
    if (right_son_ != nullptr) {
        right_son_->CleanTrie();
    }
    delete this;
}

void Trie::AddString(char16_t code, int16_t len, char16_t val) {
    Trie* vertex = this;
    while (len > -1) {
        if ((1 << len) & code) {
            if (vertex->right_son_ == nullptr) {
                vertex->right_son_ = new Trie();
                vertex->right_son_->parent_ = vertex;
            }
            vertex = vertex->right_son_;
        } else {
            if (vertex->left_son_ == nullptr) {
                vertex->left_son_ = new Trie();
                vertex->left_son_->parent_ = vertex;
            }
            vertex = vertex->left_son_;
        }
        --len;
    }
    vertex->value_ = val;
}
bool Trie::IsTerminal() const {
    return (left_son_ == nullptr);
}
char16_t Trie::GetVal() const {
    return value_;
}
Trie* Trie::GetLeft() const {
    return left_son_;
}
Trie* Trie::GetRight() const {
    return right_son_;
}
Trie* Trie::GetParent() const {
    return parent_;
}
//////////////////////////////////////////////////////////////////////////////////////////