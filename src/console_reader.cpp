#include "archiver.h"

// Программа-архиватор должна иметь следующий интерфейс командной строки:
// * `archiver -c archive_name file1 [file2 ...]` - заархивировать файлы `file1, file2, ...` и сохранить результат в
// файл `archive_name`.
// * `archiver -d archive_name` - разархивировать файлы из архива `archive_name` и положить в текущую директорию.
// * `archiver -h` - вывести справку по использованию программы.

// public
//////////////////////////////////////////////////////////////////////////////////////////
ConsoleReader::ConsoleReader(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Too few arguments!\n";
        exit(ERROR_CODE);
    }
    // argv[0] - Executable file
    // argv[1] - "archiver"
    // argv[2] - flag of use
    std::string flag = argv[1];
    if (flag == "-c") {
        is_need_archive_ = true;
        if (argc < 3) {
            std::cerr << "Too few arguments!\n";
            exit(ERROR_CODE);
        }
        // argv[2] - имя архивного файла
        archive_name_ = argv[2];
        // argv[3, ..., argc] - файлы, которые необходимо заархивировать
        for (int i = 3; i < argc; i++) {
            file_names_.push_back(argv[i]);
        }
    } else if (flag == "-d") {
        is_need_dearchive_ = true;
        if (argc < 2) {
            std::cerr << "Too few arguments!\n";
            exit(ERROR_CODE);
        }
        // argv[2] - имя архивного файла
        archive_name_ = argv[2];
    } else if (flag == "-h") {
        is_need_log_ = true;
    } else {
        std::cerr << "First argument should be flag!\n";
        exit(ERROR_CODE);
    }
}
bool ConsoleReader::IsNeedArchive() const {
    return is_need_archive_;
}
bool ConsoleReader::IsNeedDearchive() const {
    return is_need_dearchive_;
}
bool ConsoleReader::IsNeedLog() const {
    return is_need_log_;
}
const std::string ConsoleReader::GetArchiveName() const {
    return archive_name_;
}
const std::vector<std::string> ConsoleReader::GetFileNames() const {
    return file_names_;
}
//////////////////////////////////////////////////////////////////////////////////////////