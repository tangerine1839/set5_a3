//
// Created by Vika on 08.02.2026.
//

#ifndef A3_RANDOMSTREAMGEN_H
#define A3_RANDOMSTREAMGEN_H
#include <cstdint>
#include <random>

class RandomStreamGen {

public:
    RandomStreamGen(size_t n, uint64_t seed = 42, size_t maxLen = 30)
        : n_(n), rng_(seed),
          lenDist_(1, maxLen), chDist_(0, alphabetSize() - 1) {}

    // Сгенерировать поток целиком
    std::vector<std::string> genStream() {
        std::vector<std::string> s;
        s.reserve(n_);
        for (size_t i = 0; i < n_; ++i) {
            s.push_back(genOne());
        }
        return s;
    }

    // Получить префикс потока до момента времени t (в процентах)
    static std::vector<std::string> getPrefix(const std::vector<std::string>& fullStream,
                                              double percentage) {
        if (percentage <= 0.0) return {};
        if (percentage >= 1.0) return fullStream;

        size_t size = static_cast<size_t>(fullStream.size() * percentage);
        return std::vector<std::string>(
            fullStream.begin(),
            fullStream.begin() + size
        );
    }

private:
    static constexpr const char* alphabet() {
        return "abcdefghijklmnopqrstuvwxyz"
               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "0123456789"
               "-";
    }

    static constexpr size_t alphabetSize() {
        return 26 + 26 + 10 + 1;
    }

    std::string genOne() {
        size_t len = lenDist_(rng_);
        std::string out(len, '\0');
        for (size_t i = 0; i < len; ++i) {
            out[i] = alphabet()[chDist_(rng_)];
        }
        return out;
    }

    size_t n_;
    std::mt19937_64 rng_;
    std::uniform_int_distribution<size_t> lenDist_;
    std::uniform_int_distribution<size_t> chDist_;
};


#endif //A3_RANDOMSTREAMGEN_H