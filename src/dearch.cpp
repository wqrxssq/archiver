#include "archiver.h"

// public
//////////////////////////////////////////////////////////////////////////////////////////
Dearchiver::Dearchiver(std::string archive_name) {
    ByteReader reader(archive_name);

    is_end_of_archive_ = false;
    do {
        // get symbols count
        char16_t symbols_count = reader.ReadVal(MAX_CHAR_SIZE);

        // get order of symbols in canonical form
        std::vector<char16_t> alphavet(symbols_count);
        for (char16_t& c : alphavet) {
            c = reader.ReadVal(MAX_CHAR_SIZE);
        }

        // get list of lengths
        std::vector<char16_t> cnt_len;
        int counter = 0;
        char16_t symbols_left = symbols_count;
        while (symbols_left > 0 && counter++ < MAX_TRIES_BEFORE_ERROR) {
            char16_t cur_cnt = reader.ReadVal(MAX_CHAR_SIZE);
            cnt_len.push_back(cur_cnt);
            symbols_left -= cur_cnt;
        }
        if (counter == MAX_TRIES_BEFORE_ERROR || symbols_left != 0) {
            std::cerr << "Bad fomat!\n";
            exit(ERROR_CODE);
        }

        Trie* trie = BuildTrie(alphavet, cnt_len);

        // file_name
        std::string file_name;
        for (char16_t c = 0;; file_name.push_back(static_cast<char>(c))) {
            c = GetSymbol(reader, trie);
            if (c == FILENAME_END) {
                break;
            }
            if (c == ONE_MORE_FILE || c == ARCHIVE_END) {
                std::cerr << "Bad format\n";
                exit(ERROR_CODE);
            }
        }

        std::ofstream out(file_name);
        // file
        for (char16_t c = 0;;) {
            c = GetSymbol(reader, trie);
            if (c == FILENAME_END) {
                std::cerr << "Bad format\n";
                exit(ERROR_CODE);
            }
            if (c == ONE_MORE_FILE || c == ARCHIVE_END) {
                is_end_of_archive_ = (c == ARCHIVE_END);
                break;
            }
            out << static_cast<char>(c);
        }

        out.close();
        trie->CleanTrie();
    } while (!is_end_of_archive_);
}
//////////////////////////////////////////////////////////////////////////////////////////

// private
//////////////////////////////////////////////////////////////////////////////////////////
std::vector<int16_t> Dearchiver::GetLenOfSymbols(const std::vector<char16_t>& alphavet,
                                                 const std::vector<char16_t>& cnt_len) {
    std::vector<int16_t> len_of_symbols(alphavet.size());
    size_t cur = 0;
    for (size_t i = 0; i < cnt_len.size(); i++) {
        for (char16_t j = 0; j < cnt_len[i]; j++) {
            len_of_symbols[cur++] = static_cast<int16_t>(i);
        }
    }
    return len_of_symbols;
}

Trie* Dearchiver::BuildTrie(const std::vector<char16_t>& alphavet, const std::vector<char16_t>& cnt_len) {
    std::vector<int16_t> len_of_symbol = GetLenOfSymbols(alphavet, cnt_len);

    // Get canonical form and add to trie
    Trie* root = new Trie();
    std::vector<char16_t> canon_codes(alphavet.size());
    canon_codes[0] = 0;
    root->AddString(canon_codes[0], len_of_symbol[0], alphavet[0]);
    for (size_t i = 1; i < alphavet.size(); i++) {
        canon_codes[i] = canon_codes[i - 1] + 1;
        for (char16_t j = 0; j < len_of_symbol[i] - len_of_symbol[i - 1]; j++) {
            canon_codes[i] <<= 1;
        }
        root->AddString(canon_codes[i], len_of_symbol[i], alphavet[i]);
    }
    return root;
}

char16_t Dearchiver::GetSymbol(ByteReader& reader, Trie* root) {
    Trie* cur_vertex = root;
    do {
        bool bit = reader.ReadBit();
        if (!bit) {
            cur_vertex = cur_vertex->GetLeft();
        } else {
            cur_vertex = cur_vertex->GetRight();
        }
    } while (!cur_vertex->IsTerminal());
    return cur_vertex->GetVal();
}
//////////////////////////////////////////////////////////////////////////////////////////