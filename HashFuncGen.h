//
// Created by Vika on 08.02.2026.
//

#ifndef A3_HASHFUNCGEN_H
#define A3_HASHFUNCGEN_H
#include <cstdint>
#include <string>


class HashFuncGen {
public:
    explicit HashFuncGen(uint32_t seed = 0) : seed_(seed) {}

    uint32_t operator()(const std::string& s) const {
        uint32_t h = 2166136261u ^ seed_;
        for (unsigned char c : s) {
            h ^= static_cast<uint32_t>(c);
            h *= 16777619u;
        }
        return h;
    }

private:
    uint32_t seed_;
};


#endif //A3_HASHFUNCGEN_H