#ifndef PKG_H
#define PKG_H

#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cstring>

#pragma pack(push, 1)
struct PKGHeader {
    char     magic[4] = {'P', 'K', 'G', '!'}; // Paket imzası
    char     packageName[64] = {0};          
    uint16_t versionMajor = 1;
    uint16_t versionMinor = 0;
    uint8_t  architecture = 64;               
    uint32_t payloadSize = 0;                 
};
#pragma pack(pop)

class PKGManager {
public:
    static bool createPackage(const std::string& outputFile, const std::string& pkgName, 
                             uint8_t arch, const std::vector<uint8_t>& payload) {
        PKGHeader header;
        std::strncpy(header.packageName, pkgName.c_str(), sizeof(header.packageName) - 1);
        header.architecture = arch;
        header.payloadSize = static_cast<uint32_t>(payload.size());

        std::ofstream out(outputFile, std::ios::binary);
        if (!out) return false;

        out.write(reinterpret_cast<const char*>(&header), sizeof(PKGHeader));
        out.write(reinterpret_cast<const char*>(payload.data()), payload.size());
        return true;
    }

    static bool readPackage(const std::string& inputFile, PKGHeader& outHeader, std::vector<uint8_t>& outPayload) {
        std::ifstream in(inputFile, std::ios::binary);
        if (!in) return false;

        in.read(reinterpret_cast<char*>(&outHeader), sizeof(PKGHeader));

        if (outHeader.magic[0] != 'P' || outHeader.magic[1] != 'K' || 
            outHeader.magic[2] != 'G' || outHeader.magic[3] != '!') {
            return false;
        }

        outPayload.resize(outHeader.payloadSize);
        in.read(reinterpret_cast<char*>(outPayload.data()), outHeader.payloadSize);
        return true;
    }
};

#endif