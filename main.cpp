#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include "Generator.h"
#include "json/json.hpp"

void visualize() {
    char memory[51]{};
    memory[50] = '\0';
    for (int i = 0; i < 5; ++i) {
        Generator::get(i * 50, (i + 1) * 50, memory);
        std::cout << memory << std::endl;
    }
}

void performanceTest() {
    constexpr auto bfr = 100 * 1024UL;
    constexpr auto size = 2 * 1024UL * 1024UL * 1024UL;

    auto *memory = new char[bfr];
    auto read = 0UL;

    auto t_start = std::chrono::high_resolution_clock::now();
    Generator generator(0);
    while (read < size) {
        generator.read(memory, bfr);
        read += bfr;
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    delete[] memory;

    std::cout << std::chrono::duration<double, std::milli>(t_end - t_start).count() << std::endl;
}

inline ULong min(ULong a, ULong b) {
    return a > b ? b : a;
}

using json = nlohmann::json;
using std::string;
using std::map;
using std::vector;

int main() {
    json data;
    std::cin >> data;
    auto begin = 0ULL, end = 100ULL * 1024ULL * 1024ULL;

    auto getParams = data["GET"];
    auto startIter = getParams.find("start");
    auto endIter = getParams.find("end");
    if (startIter != getParams.end())
        begin = std::stoull(static_cast<string>(*startIter));
    if (endIter != getParams.end())
        end = std::stoull(static_cast<string>(*endIter));

    Generator generator(begin);
    map<string, vector<string>> headers;
    headers["Content-Type"] = {"text/html"};
//    headers["Content-Type"] = {"application/octet-stream"};
//    headers["Content-Disposition"] = {"attachment; filename=\"numbers.txt\""};
    json meta{
            {"responseCode", 200},
            {"headers",      headers},
            {"length",       end - begin},
            {"data",         "inline"}
    };
    std::cout << meta.dump() << "\n\n" << std::endl;
    constexpr auto BUFFER_SIZE = 1024U * 10U;
    const auto totalBytes = end - begin;
    auto read = 0ULL;
    char buffer[BUFFER_SIZE + 1];
    while (read < totalBytes) {
        auto bytesToWrite = min(BUFFER_SIZE, totalBytes - read);
        generator.read(buffer, bytesToWrite);
        read += bytesToWrite;
        buffer[bytesToWrite] = '\0';
        std::cout << buffer;
    }
    std::cout.flush();
    return 0;
}
