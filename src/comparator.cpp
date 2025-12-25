#include "comparator.h"

Comparator::Comparator() {}
    //nothing here for now

Comparator::~Comparator() {}
void Comparator::compare_chunk(std::vector<std::array<float,6>> chunk) {
    //check in stored chunks for "similar chunks"
    //nts for future me: rn the issue is comparing chunks
    //if over SIMILARITY_THRESHOLD, add it to stored
    storedChunks.push_back(chunk);
}