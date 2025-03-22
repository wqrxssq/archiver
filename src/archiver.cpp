#include "archiver.h"

int main(int argc, char** argv) {
    // считаем данные
    ConsoleReader reader(argc, argv);

    if (reader.IsNeedArchive()) {
        Archiver arch(reader.GetArchiveName(), reader.GetFileNames());
    } else if (reader.IsNeedDearchive()) {
        Dearchiver de_arch(reader.GetArchiveName());
    } else if (reader.IsNeedLog()) {
        // Logger logger();
    } else {
        std::cerr << "Something went wrong!\n";
        return ERROR_CODE;
    }
}
