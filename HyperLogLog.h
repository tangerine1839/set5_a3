//
// Created by Vika on 09.02.2026.
//
#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <cstdint>
#include <string>

template<typename Hash>
class HyperLogLog {
private:
    int b_;                       // Количество бит для индекса субпотока
    std::size_t m_;               // Количество регистров: m = 2^b
    std::vector<uint8_t> M_;      // Регистры
    Hash hashFunc_;               // Хеш-функция

    // Корректировочные константы в зависимости от m
    double alpha() const {
        if (m_ == 16) return 0.673;
        if (m_ == 32) return 0.697;
        if (m_ == 64) return 0.709;
        return 0.7213 / (1.0 + 1.079 / m_);
    }

    // Подсчет ведущих нулей в 32-битном числе
    static uint8_t countLeadingZeros(uint32_t x) {
        if (x == 0) return 32;

        uint8_t count = 0;
        for (int i = 31; i >= 0; i--) {
            if ((x >> i) & 1) {
                break;
            }
            count++;
        }
        return count;
    }

public:
    HyperLogLog(int b, const Hash& hash)
        : b_(b), m_(1 << b), M_(m_, 0), hashFunc_(hash) {}

    void add(const std::string& element) {
        uint32_t hash = hashFunc_(element);

        std::size_t j = hash >> (32 - b_);

        uint32_t w = hash << b_;

        uint8_t zeros = countLeadingZeros(w) + 1;

        if (zeros > M_[j]) {
            M_[j] = zeros;
        }
    }

    double estimate() const {
        double sum = 0.0;
        int zeroRegisters = 0;

        for (std::size_t j = 0; j < m_; ++j) {
            sum += 1.0 / (1ULL << M_[j]);
            if (M_[j] == 0) {
                zeroRegisters++;
            }
        }

        double E = alpha() * m_ * m_ / sum;

        if (E <= 2.5 * m_) {
            if (zeroRegisters > 0) {
                E = m_ * std::log(static_cast<double>(m_) / zeroRegisters);
            }
        }

        return E;
    }
};

#endif // HYPERLOGLOG_H