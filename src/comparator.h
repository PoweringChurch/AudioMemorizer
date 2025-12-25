#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>
#include <array>

constexpr float SIMILARITY_THRESHOLD;

class Comparator {
public:
    Comparator();
    ~Comparator();
    void compare_chunk(std::vector<std::array<float,6>>);
private:
    std::vector<std::vector<std::array<float,6>>> storedChunks;
};

#endif