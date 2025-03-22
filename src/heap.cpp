#include "archiver.h"

bool Cmp::operator()(const std::pair<int, Trie*> left, const std::pair<int, Trie*> right) const {
    return left.first > right.first || (left.first == right.first && left.second->GetVal() > right.second->GetVal());
}