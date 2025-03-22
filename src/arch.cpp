#include "archiver.h"
#include "heap.h"

// public
//////////////////////////////////////////////////////////////////////////////////////////
Archiver::Archiver(const std::string& archive_name, const std::vector<std::string>& file_names) {
    BytePrinter printer(archive_name);
    for (const std::string& file_name : file_names) {
        std::string filename_wo_path = std::filesystem::path(file_name).filename().string();

        // Open an input stream with a file
        in_.open(file_name);
        if (!in_.is_open()) {
            std::cerr << "No such file!\n";
            exit(ERROR_CODE);
        }

        std::map<char16_t, int> frequencies;

        // Add file_name(without path info!)
        for (const char c : filename_wo_path) {
            frequencies[static_cast<char16_t>(c)]++;
        }

        // Add text file
        char cur_symbol = '0';
        while (in_.get(cur_symbol)) {
            frequencies[static_cast<char16_t>(cur_symbol)]++;
        }

        // Add special symbols FILENAME_END(256), ONE_MORE_FILE(257), ARCHIVE_END(258)
        frequencies[static_cast<char16_t>(FILENAME_END)]++;
        frequencies[static_cast<char16_t>(ONE_MORE_FILE)]++;
        frequencies[static_cast<char16_t>(ARCHIVE_END)]++;

        in_.close();

        // Build trie by "HuffmanAlgorithm"
        Trie* trie = HuffmanAlgorithm(frequencies);
        std::vector<Symbol> len_symbols;
        len_symbols = GetLens(trie);

        trie->CleanTrie();

        // Get codes to canonical
        std::map<char16_t, Symbol> canonical_codes = ConvertCodesToCanonical(len_symbols);

        // Output
        // Output number of characters in alphabet
        printer.Print(canonical_codes.size(), MAX_CHAR_SIZE);

        // Output symbols in the order of canonical codes
        char16_t max_symbol_code_size = 0;
        for (auto [c, len] : len_symbols) {
            max_symbol_code_size = std::max(len, max_symbol_code_size);
            printer.Print(c, MAX_CHAR_SIZE);
        }

        // Make list of code lengths
        std::vector<char16_t> cnt_len(max_symbol_code_size);
        for (auto [c, len] : len_symbols) {
            cnt_len[len - 1]++;
        }

        // Output this list
        for (size_t i = 0; i < max_symbol_code_size; i++) {
            printer.Print(cnt_len[i], MAX_CHAR_SIZE);
        }

        // Output filename
        for (const char c : filename_wo_path) {
            printer.Print(canonical_codes[c].c, canonical_codes[c].len);
        }

        // Output FILENAME_END
        printer.Print(canonical_codes[FILENAME_END].c, canonical_codes[FILENAME_END].len);

        // Open the input stream with the file again
        in_.open(file_name);
        // Output text
        while (in_.get(cur_symbol)) {
            printer.Print(canonical_codes[cur_symbol].c, canonical_codes[cur_symbol].len);
        }
        in_.close();

        // Finish archive or add another file
        if (file_name == file_names.back()) {
            printer.Print(canonical_codes[ARCHIVE_END].c, canonical_codes[ARCHIVE_END].len);
        } else {
            printer.Print(canonical_codes[ONE_MORE_FILE].c, canonical_codes[ONE_MORE_FILE].len);
        }
    }
    printer.EndPrint();
}
//////////////////////////////////////////////////////////////////////////////////////////

// private
//////////////////////////////////////////////////////////////////////////////////////////
Trie* Archiver::HuffmanAlgorithm(const std::map<char16_t, int> frequencies) {
    // build heap of frequencies
    Heap<std::pair<int, Trie*>, Cmp> q;
    for (auto [c, freq] : frequencies) {
        Trie* node = new Trie(c);
        q.Push({freq, node});
    }
    // size is always >= 2
    while (q.Size() > 1) {
        auto [freq_left, left] = q.Top();
        q.Pop();
        auto [freq_right, right] = q.Top();
        q.Pop();
        Trie* node = new Trie(left, right);
        q.Push({freq_left + freq_right, node});
    }
    // Last vertex is root
    return q.Top().second;
}

// Dfs trie to find lens
void Archiver::Dfs(std::vector<Symbol>& lens, Trie* v, char16_t depth) {
    if (v->IsTerminal()) {
        lens.push_back(Symbol{v->GetVal(), depth});
    } else {
        v = v->GetLeft();
        Dfs(lens, v, depth + 1);
        v = v->GetParent();

        v = v->GetRight();
        Dfs(lens, v, depth + 1);
        v = v->GetParent();
    }
}
// Get lens of codes
std::vector<Archiver::Symbol> Archiver::GetLens(Trie* root) {
    std::vector<Symbol> lens;
    Dfs(lens, root);
    return lens;
}

std::map<char16_t, Archiver::Symbol> Archiver::ConvertCodesToCanonical(std::vector<Symbol>& lens) {
    // Sort by lengths + alphavet
    std::sort(lens.begin(), lens.end(),
              [](Symbol& left, Symbol& right) { return std::tie(left.len, left.c) < std::tie(right.len, right.c); });

    // Get canon codes
    std::vector<char16_t> canon_codes(lens.size());
    canon_codes[0] = 0;
    for (size_t i = 1; i < lens.size(); i++) {
        canon_codes[i] = canon_codes[i - 1] + 1;
        for (size_t j = 0; j < lens[i].len - lens[i - 1].len; j++) {
            canon_codes[i] <<= 1;
        }
    }

    // Get coding table
    std::map<char16_t, Symbol> coding_table;
    for (size_t i = 0; i < lens.size(); i++) {
        coding_table[lens[i].c] = {canon_codes[i], lens[i].len};
    }
    return coding_table;
}
//////////////////////////////////////////////////////////////////////////////////////////