#include "archiver.h"

// public
//////////////////////////////////////////////////////////////////////////////////////////
BytePrinter::BytePrinter(const std::string& archive_name) : value_(0), pos_(BYTE_SIZE - 1) {
    out_.open(archive_name);
}
BytePrinter::~BytePrinter() {
    out_.close();
}

void BytePrinter::Print(char16_t c, char16_t len) {
    for (int16_t mask = static_cast<int16_t>(1 << (len - 1)); mask; mask >>= 1) {
        Out((mask & c) > 0);
    }
}
void BytePrinter::EndPrint() {
    if (pos_ != BYTE_SIZE - 1) {
        Flush();
    }
}
//////////////////////////////////////////////////////////////////////////////////////////

// private
//////////////////////////////////////////////////////////////////////////////////////////
void BytePrinter::Out(bool bit) {
    int new_value = static_cast<int>(value_) | (static_cast<int>(bit) << pos_);
    value_ = static_cast<char>(new_value);
    --pos_;
    if (pos_ == -1) {
        Flush();
    }
}
void BytePrinter::Flush() {
    out_ << value_;
    value_ = 0;
    pos_ = BYTE_SIZE - 1;
}
//////////////////////////////////////////////////////////////////////////////////////////