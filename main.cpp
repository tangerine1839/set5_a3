#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <unordered_set>

#include "RandomStreamGen.h"
#include "HashFuncGen.h"
#include "HyperLogLog.h"

// Точный подсчет уникальных элементов в префиксе
std::size_t countExactUnique(std::vector<std::string>& stream, std::size_t prefixSize) {
    std::unordered_set<std::string> uniqueSet;
    uniqueSet.reserve(prefixSize);

    for (std::size_t i = 0; i < prefixSize; ++i) {
        uniqueSet.insert(stream[i]);
    }
    return uniqueSet.size();
}

// Среднее значение
double calculateMean(std::vector<double>& values) {
    if (values.empty()) return 0.0;
    double sum = 0.0;
    for (double v : values) sum += v;
    return sum / values.size();
}

// Стандартное отклонение
double calculateStdDev(std::vector<double>& values) {
    if (values.size() < 2) return 0.0;

    double mean = calculateMean(values);
    double sumSq = 0.0;

    for (double v : values) {
        double diff = v - mean;
        sumSq += diff * diff;
    }

    return std::sqrt(sumSq / (values.size() - 1));
}

int main() {
    const int B = 10;                     // Количество бит для индекса
    const std::size_t STREAM_SIZE = 50000; // Размер потока
    const std::size_t NUM_STREAMS = 30;    // Количество потоков
    const std::size_t STEP_PERCENT = 10;   // Шаг в процентах

    std::vector<std::size_t> timePoints;
    for (std::size_t p = STEP_PERCENT; p <= 100; p += STEP_PERCENT) {
        std::size_t k = (STREAM_SIZE * p) / 100;
        if (k == 0) k = 1;
        timePoints.push_back(k);
    }

    std::vector<std::vector<double>> hllEstimates(timePoints.size());
    std::vector<std::vector<double>> exactCounts(timePoints.size());

    for (std::size_t streamId = 0; streamId < NUM_STREAMS; ++streamId) {
        RandomStreamGen gen(STREAM_SIZE, 1000 + streamId);
        std::vector<std::string> stream = gen.genStream();

        HashFuncGen hashFunc(777);
        HyperLogLog<HashFuncGen> hll(B, hashFunc);

        std::size_t processed = 0;

        // Анализ в каждый момент времени
        for (std::size_t t = 0; t < timePoints.size(); ++t) {
            std::size_t targetSize = timePoints[t];

            for (std::size_t i = processed; i < targetSize; ++i) {
                hll.add(stream[i]);
            }
            processed = targetSize;

            std::size_t exact = countExactUnique(stream, targetSize);

            double estimate = hll.estimate();

            exactCounts[t].push_back(static_cast<double>(exact));
            hllEstimates[t].push_back(estimate);
        }

        if ((streamId + 1) % 5 == 0) {
            std::cout << "Обработано потоков: " << (streamId + 1) << " из " << NUM_STREAMS << std::endl;
        }
    }

    std::vector<double> meanExact(timePoints.size());
    std::vector<double> meanEstimate(timePoints.size());
    std::vector<double> stdEstimate(timePoints.size());

    for (std::size_t t = 0; t < timePoints.size(); ++t) {
        meanExact[t] = calculateMean(exactCounts[t]);
        meanEstimate[t] = calculateMean(hllEstimates[t]);
        stdEstimate[t] = calculateStdDev(hllEstimates[t]);
    }

    std::ofstream out("results.csv");

    out << "time_index,prefix_size,exact_mean,estimate_mean,estimate_std,"
        << "estimate_minus_std,estimate_plus_std\n";

    for (std::size_t t = 0; t < timePoints.size(); ++t) {
        double minusStd = meanEstimate[t] - stdEstimate[t];
        double plusStd = meanEstimate[t] + stdEstimate[t];

        out << t << ","
            << timePoints[t] << ","
            << meanExact[t] << ","
            << meanEstimate[t] << ","
            << stdEstimate[t] << ","
            << minusStd << ","
            << plusStd << "\n";
    }

    out.close();

    std::cout << "\nДанные сохранены в results.csv" << std::endl;
    std::cout << "Всего моментов времени: " << timePoints.size() << std::endl;

    return 0;
}