#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "pkg.h"

void printHelp() {
    std::cout << "========================================================\n";
    std::cout << "             Remove Package (RMPKG) v1.0                \n";
    std::cout << "========================================================\n";
    std::cout << "Kullanım:\n";
    std::cout << "  ./rmpkg -help\n";
    std::cout << "  ./rmpkg -l -<paket_adı.pkg>\n";
    std::cout << "  ./rmpkg -x -<paket_adı.pkg> -o -<hedef_klasör>\n\n";
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

    std::string mode = argv[1];

    // Listeleme Modu (-l)
    if (mode == "-l" && argc >= 3) {
        std::string pkgFile = cleanFlag(argv[2]);
        PKGHeader header;
        std::vector<uint8_t> payload;

        if (PKGManager::readPackage(pkgFile, header, payload)) {
            std::cout << "----------------------------------------\n";
            std::cout << "PAKET BİLGİSİ: " << pkgFile << "\n";
            std::cout << "----------------------------------------\n";
            std::cout << "Paket Adı : " << header.packageName << "\n";
            std::cout << "Mimari    : " << (int)header.architecture << "-bit\n";
            std::cout << "Boyut     : " << header.payloadSize << " Bayt\n";
            std::cout << "----------------------------------------\n";
        } else {
            std::cout << "[HATA] Paket okunaz veya geçersiz dosya biçimi!\n";
        }
        return 0;
    }

    // Çıkarma Modu (-x)
    if (mode == "-x" && argc >= 5) {
        std::string pkgFile = cleanFlag(argv[2]);
        std::string outFlag = argv[3];
        std::string outDir = cleanFlag(argv[4]);

        if (outFlag != "-o") {
            std::cout << "[HATA] Hedef klasör için '-o' bayrağını kullanmalısın!\n";
            return 1;
        }

        PKGHeader header;
        std::vector<uint8_t> payload;

        if (PKGManager::readPackage(pkgFile, header, payload)) {
            std::string outPath = outDir + "/extracted_payload.bin";
            std::ofstream outFile(outPath, std::ios::binary);
            if (outFile) {
                outFile.write(reinterpret_cast<const char*>(payload.data()), payload.size());
                std::cout << "[RMPKG] Paket başarıyla " << outPath << " konumuna çıkarıldı.\n";
            }
        } else {
            std::cout << "[HATA] Paket çıkarılamadı!\n";
        }
        return 0;
    }

    std::cout << "[HATA] Eksik veya hatalı komut! Detay için: ./rmpkg -help\n";
    return 1;
}