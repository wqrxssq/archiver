#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#define ERROR_CODE 111
#define BYTE_SIZE 8
#define MAX_TRIES_BEFORE_ERROR 500
#define MAX_CHAR_SIZE 9
#define FILENAME_END 256
#define ONE_MORE_FILE 257
#define ARCHIVE_END 258

class Trie {
public:
    Trie();
    explicit Trie(char16_t c);

    // merge two Tries
    Trie(Trie* left, Trie* right);

    void CleanTrie();

    void AddString(char16_t code, int16_t len, char16_t val);

    bool IsTerminal() const;
    char16_t GetVal() const;
    Trie* GetLeft() const;
    Trie* GetRight() const;
    Trie* GetParent() const;

private:
    Trie* parent_;
    Trie* left_son_;
    Trie* right_son_;
    char16_t value_;
};

// Comparator for heap
struct Cmp {
    bool operator()(const std::pair<int, Trie*> left, const std::pair<int, Trie*> right) const;
};

class BytePrinter {
public:
    explicit BytePrinter(const std::string& archive_name);
    ~BytePrinter();

    void Print(char16_t c, char16_t len);
    void EndPrint();

private:
    std::ofstream out_;
    char value_;
    int16_t pos_;

    void Out(bool bit);
    void Flush();
};

class ByteReader {
public:
    explicit ByteReader(const std::string& archive_name);
    ~ByteReader();

    bool ReadBit();

    // Read "len" bits and write it to char16_t
    char16_t ReadVal(const int16_t& len);

private:
    std::ifstream in_;
    int16_t pos_;
    char byte_;

    void GetNewByte();
};

class ConsoleReader {
public:
    ConsoleReader(int argc, char** argv);

    bool IsNeedArchive() const;
    bool IsNeedDearchive() const;
    bool IsNeedLog() const;
    const std::string GetArchiveName() const;
    const std::vector<std::string> GetFileNames() const;

private:
    bool is_need_archive_ = false;
    bool is_need_dearchive_ = false;
    bool is_need_log_ = false;
    std::vector<std::string> file_names_;
    std::string archive_name_;
};

class Archiver {
public:
    Archiver(const std::string& archive_name, const std::vector<std::string>& file_names);

private:
    struct Symbol {
        char16_t c;
        char16_t len;
    };

    std::ifstream in_;

    Trie* HuffmanAlgorithm(const std::map<char16_t, int> frequencies);

    // Dfs trie to find lens
    void Dfs(std::vector<Symbol>& lens, Trie* v, char16_t depth = 0);

    // Get lens of codes
    std::vector<Symbol> GetLens(Trie* root);

    std::map<char16_t, Symbol> ConvertCodesToCanonical(std::vector<Symbol>& lens);
};

class Dearchiver {
public:
    explicit Dearchiver(std::string archive_name);

private:
    bool is_end_of_archive_;

    std::vector<int16_t> GetLenOfSymbols(const std::vector<char16_t>& alphavet, const std::vector<char16_t>& cnt_len);

    Trie* BuildTrie(const std::vector<char16_t>& alphavet, const std::vector<char16_t>& cnt_len);

    // Get symbol using trie
    char16_t GetSymbol(ByteReader& reader, Trie* root);
};

class Logger {};
