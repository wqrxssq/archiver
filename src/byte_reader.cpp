#include "archiver.h"

// public
//////////////////////////////////////////////////////////////////////////////////////////
ByteReader::ByteReader(const std::string& archive_name) : pos_(-1), byte_(0) {
    in_.open(archive_name);
    if (!in_.is_open()) {
        std::cerr << "No such file!\n";
        exit(ERROR_CODE);
    }
}
ByteReader::~ByteReader() {
    in_.close();
}
bool ByteReader::ReadBit() {
    if (pos_ == -1) {
        GetNewByte();
    }
    return ((1 << pos_--) & byte_) > 0;
}
char16_t ByteReader::ReadVal(const int16_t& len) {
    char16_t val = 0;
    for (int16_t pos = static_cast<int16_t>(len - 1); pos > -1; --pos) {
        val |= (ReadBit() << pos);
    }
    return val;
}
//////////////////////////////////////////////////////////////////////////////////////////

// private
//////////////////////////////////////////////////////////////////////////////////////////
void ByteReader::GetNewByte() {
    if (in_.eof()) {
        std::cerr << "Bad format!\n";
        exit(ERROR_CODE);
    }
    in_.get(byte_);
    pos_ = BYTE_SIZE - 1;
}
//////////////////////////////////////////////////////////////////////////////////////////