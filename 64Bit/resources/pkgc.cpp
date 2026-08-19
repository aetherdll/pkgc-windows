#include <iostream>
#include <string>
#include <vector>
#include "pkg.h"

void printHelp() {
    std::cout << "========================================================\n";
    std::cout << "             Package Compiler (PKGC) v1.0               \n";
    std::cout << "========================================================\n";
    std::cout << "Kullanım:\n";
    std::cout << "  ./pkgc -help\n";
    std::cout << "  ./pkgc -32 -<klasör_adı> -o -<paket_adı.pkg>\n";
    std::cout << "  ./pkgc -64 -<klasör_adı> -o -<paket_adı.pkg>\n\n";
    std::cout << "Dokümantasyon:\n";
    std::cout << "  https://github.com/aetherdll/pkgc/docs\n";
    std::cout << "========================================================\n";
}

std::string cleanFlag(const std::string& arg) {
    return (!arg.empty() && arg[0] == '-') ? arg.substr(1) : arg;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || std::string(argv[1]) == "-help") {
        printHelp();
        return 0;
    }

    uint8_t arch = 64;
    std::string folderName = "";
    std::string outputFile = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-32") {
            arch = 32;
        } else if (arg == "-64") {
            arch = 64;
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = cleanFlag(argv[++i]);
        } else if (arg[0] == '-' && folderName.empty()) {
            folderName = cleanFlag(arg);
        }
    }

    if (folderName.empty() || outputFile.empty()) {
        std::cout << "[HATA] Parametreler eksik! Detay için: ./pkgc -help\n";
        return 1;
    }

    std::vector<uint8_t> samplePayload = { 0x48, 0x31, 0xC0, 0xC3 };

    if (PKGManager::createPackage(outputFile, folderName, arch, samplePayload)) {
        std::cout << "[PKGC] '" << outputFile << "' paketi başarıyla derlendi.\n";
    } else {
        std::cout << "[HATA] Paket oluşturulurken bir sorun çıktı.\n";
    }

    return 0;
}